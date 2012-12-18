#include <v8.h>
#include <node.h>
#include <node_buffer.h>

using namespace v8;

extern "C" {
#include <DG/dg.h>
}

Handle<Value> CreateSurface (const Arguments& args)
{
    HandleScope scope;
    Local<Object> options = args[2]->ToObject();
    Local<Value> v;
    int alpha = 8, stencil = 0;
    v = options->Get(String::NewSymbol("alpha"));
    if (!v->IsUndefined()) alpha = v->IntegerValue();
    v = options->Get(String::NewSymbol("stencil"));
    if (!v->IsUndefined()) stencil = v->IntegerValue();
    int attr[] = {
        DG_RED_CHANNEL, 8,
        DG_GREEN_CHANNEL, 8,
        DG_BLUE_CHANNEL, 8,
        DG_ALPHA_CHANNEL, alpha,
        DG_STENCIL_CHANNEL, stencil,
        0
    };
    dg_surface s = dgCreateSurface(
        args[0]->IntegerValue(),
        args[1]->IntegerValue(),
        attr
    );
    return scope.Close(External::Wrap(s));
}

Handle<Value> SwapBuffers (const Arguments& args)
{
    HandleScope scope;
    dgSwapBuffers((dg_surface)External::Unwrap(args[0]));
    return scope.Close(Undefined());
}

Handle<Value> SurfaceId (const Arguments& args)
{
    HandleScope scope;
    const char* a = dgSurfaceId((dg_surface)External::Unwrap(args[0]));
    return scope.Close(String::New(a, DG_SURFACE_ID_LENGTH));
}

Handle<Value> DestroySurface (const Arguments& args)
{
    HandleScope scope;
    dgDestroySurface((dg_surface)External::Unwrap(args[0]));
    return scope.Close(Undefined());
}

Handle<Value> CreateContext (const Arguments& args)
{
    HandleScope scope;
    dg_context c = dgCreateContext(args[0]->IntegerValue());
    return scope.Close(External::Wrap(c));
}

Handle<Value> DestroyContext (const Arguments& args)
{
    HandleScope scope;
    dgDestroyContext((dg_context)External::Unwrap(args[0]));
    return scope.Close(Undefined());
}

Handle<Value> MakeCurrent (const Arguments& args)
{
    HandleScope scope;
    dgMakeCurrent(
        (dg_surface)External::Unwrap(args[0]),
        (dg_surface)External::Unwrap(args[1]),
        (dg_context)External::Unwrap(args[2])
    );
    return scope.Close(Undefined());
}

Handle<Value> GetCurrent (const Arguments& args)
{
    HandleScope scope;
    dg_surface draw;
    dg_surface read;
    dg_context context;
    dgGetCurrent(&draw, &read, &context);
    Local<Object> r = Object::New();
    r->Set(String::NewSymbol("draw"), External::Wrap(draw));
    r->Set(String::NewSymbol("read"), External::Wrap(read));
    r->Set(String::NewSymbol("context"), External::Wrap(context));
    return scope.Close(r);
}

Handle<Value> OpenDisplayBRCM (const Arguments& args)
{
    HandleScope scope;
    dg_display_brcm d = dgOpenDisplayBRCM(args[0]->IntegerValue());
    return scope.Close(External::Wrap(d));
}

Handle<Value> CreateFullscreenSurfaceBRCM (const Arguments& args)
{
    HandleScope scope;
    Local<Object> options = args[6]->ToObject();
    Local<Value> v;
    int dst_width = args[3]->IntegerValue();
    int dst_height = args[4]->IntegerValue();
    int width = dst_width;
    int height = dst_height;
    int alpha = 8, stencil = 0;
    v = options->Get(String::NewSymbol("alpha"));
    if (!v->IsUndefined()) alpha = v->IntegerValue();
    v = options->Get(String::NewSymbol("stencil"));
    if (!v->IsUndefined()) stencil = v->IntegerValue();
    v = options->Get(String::NewSymbol("width"));
    if (!v->IsUndefined()) width = v->IntegerValue();
    v = options->Get(String::NewSymbol("height"));
    if (!v->IsUndefined()) height = v->IntegerValue();
    int attr[] = {
        DG_RED_CHANNEL, 8,
        DG_GREEN_CHANNEL, 8,
        DG_BLUE_CHANNEL, 8,
        DG_ALPHA_CHANNEL, alpha,
        DG_STENCIL_CHANNEL, stencil,
        0
    };
    dg_surface s = dgCreateFullscreenSurfaceBRCM2(
        (dg_display_brcm)External::Unwrap(args[0]),
        width, height,
        args[1]->IntegerValue(),
        args[2]->IntegerValue(),
        dst_width, dst_height,
        args[5]->IntegerValue(),
        attr
    );
    return scope.Close(External::Wrap(s));
}

Handle<Value> FullscreenResolutionBRCM(const Arguments& args)
{
    HandleScope scope;
    const int* wh = dgFullscreenResolutionBRCM((dg_display_brcm)External::Unwrap(args[0]));
    Local<Object> r = Object::New();
    r->Set(String::NewSymbol("width"), Integer::New(wh[0]));
    r->Set(String::NewSymbol("height"), Integer::New(wh[1]));
    return scope.Close(r);
}

Handle<Value> CloseDisplayBRCM (const Arguments& args)
{
    HandleScope scope;
    dgCloseDisplayBRCM((dg_display_brcm)External::Unwrap(args[0]));
    return scope.Close(Undefined());
}

Handle<Value> GetError (const Arguments& args)
{
    HandleScope scope;
    return scope.Close(Integer::New(dgGetError()));
}

static void Init(Handle<Object> target)
{
    if (!dgValidVersion(1, 1)) {
        ThrowException(String::NewSymbol("dg-1.1 unavailable"));
    }

    target->Set(String::NewSymbol("createSurface"), FunctionTemplate::New(CreateSurface)->GetFunction());
    target->Set(String::NewSymbol("swapBuffers"), FunctionTemplate::New(SwapBuffers)->GetFunction());
    target->Set(String::NewSymbol("surfaceId"), FunctionTemplate::New(SurfaceId)->GetFunction());
    target->Set(String::NewSymbol("destroySurface"), FunctionTemplate::New(DestroySurface)->GetFunction());
    target->Set(String::NewSymbol("createContext"), FunctionTemplate::New(CreateContext)->GetFunction());
    target->Set(String::NewSymbol("destroyContext"), FunctionTemplate::New(DestroyContext)->GetFunction());
    target->Set(String::NewSymbol("makeCurrent"), FunctionTemplate::New(MakeCurrent)->GetFunction());
    target->Set(String::NewSymbol("getCurrent"), FunctionTemplate::New(GetCurrent)->GetFunction());
    target->Set(String::NewSymbol("openDisplayBRCM"), FunctionTemplate::New(OpenDisplayBRCM)->GetFunction());
    target->Set(String::NewSymbol("createFullscreenSurfaceBRCM"), FunctionTemplate::New(CreateFullscreenSurfaceBRCM)->GetFunction());
    target->Set(String::NewSymbol("fullscreenResolutionBRCM"), FunctionTemplate::New(FullscreenResolutionBRCM)->GetFunction());
    target->Set(String::NewSymbol("closeDisplayBRCM"), FunctionTemplate::New(CloseDisplayBRCM)->GetFunction());
    target->Set(String::NewSymbol("getError"), FunctionTemplate::New(GetError)->GetFunction());
    target->Set(String::NewSymbol("OPENVG"), Integer::New(DG_OPENVG));
    target->Set(String::NewSymbol("OPENGL_ES_2"), Integer::New(DG_OPENGL_ES_2));
}
NODE_MODULE(dg, Init);
