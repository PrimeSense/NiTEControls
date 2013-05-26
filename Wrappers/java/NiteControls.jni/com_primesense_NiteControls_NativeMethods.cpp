//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "com_primesense_NiteControls_NativeMethods.h"
#include <XnVCNITE.h>

#include <map>

//#define DEBUG 1

#if DEBUG && defined(ANDROID)
#include <android/log.h>
#  define  LOGD(x...)  __android_log_print(ANDROID_LOG_INFO,"NITE-JNI",x)
#  define  LOGE(x...)  __android_log_print(ANDROID_LOG_ERROR,"NITE-JNI",x)
#else
#  define  LOGD(...)  do {} while (0)
#  define  LOGE(...)  do {} while (0)
#endif

//---------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------
JavaVM* g_pVM = NULL;

//---------------------------------------------------------------------------
// Utilities
//---------------------------------------------------------------------------
void SetOutArgObjectValue(JNIEnv* env, jobject p, jobject value)
{
	jclass cls = env->GetObjectClass(p);
	jfieldID fieldID = env->GetFieldID(cls, "value", "Ljava/lang/Object;");
	env->SetObjectField(p, fieldID, value);
}

void SetOutArgDoubleValue(JNIEnv* env, jobject p, double value)
{
	jclass cls = env->FindClass("java/lang/Double");
	jmethodID ctor = env->GetMethodID(cls, "<init>", "(D)V");
	SetOutArgObjectValue(env, p, env->NewObject(cls, ctor, value));
}

void SetOutArgLongValue(JNIEnv* env, jobject p, XnUInt64 value)
{
	jclass cls = env->FindClass("java/lang/Long");
	jmethodID ctor = env->GetMethodID(cls, "<init>", "(J)V");
	SetOutArgObjectValue(env, p, env->NewObject(cls, ctor, value));
}

void SetOutArgPointerValue(JNIEnv* env, jobject p, const void* value)
{
	SetOutArgLongValue(env, p, (jlong)value);
}
void SetOutArgObjectIDValue(JNIEnv* env, jobject p, XnVObjectID id)
{
	SetOutArgLongValue(env, p, (jlong)p);
}
void SetOutArgIntValue(JNIEnv* env, jobject p, int value)
{
	jclass cls = env->FindClass("java/lang/Integer");
	jmethodID ctor = env->GetMethodID(cls, "<init>", "(I)V");
	SetOutArgObjectValue(env, p, env->NewObject(cls, ctor, value));
}

void SetOutArgFloatValue(JNIEnv* env, jobject p, float value)
{
	jclass cls = env->FindClass("java/lang/Float");
	jmethodID ctor = env->GetMethodID(cls, "<init>", "(F)V");
	SetOutArgObjectValue(env, p, env->NewObject(cls, ctor, value));
}
void SetOutArgBooleanValue(JNIEnv* env, jobject p, XnBool value)
{
	jclass cls = env->FindClass("java/lang/Boolean");
	jmethodID ctor = env->GetMethodID(cls, "<init>", "(Z)V");
	SetOutArgObjectValue(env, p, env->NewObject(cls, ctor, value!=0));
}

void SetOutArgStringValue(JNIEnv* env, jobject p, const XnChar* value)
{
	SetOutArgObjectValue(env, p, env->NewStringUTF(value));
}

jobject CreateObjectFromPoint3D(JNIEnv* pEnv, const XnPoint3D* pPoint)
{
	jclass PointCls = pEnv->FindClass("org/openni/Point3D");
	jmethodID PointCtor = pEnv->GetMethodID(PointCls, "<init>", "(FFF)V");

	return pEnv->NewObject(PointCls, PointCtor, pPoint->X, pPoint->Y, pPoint->Z);
}
jobject CreateObjectFromHandPointContext(JNIEnv* pEnv, const XnVHandPointContext* pHand)
{
	jclass HandPointContextCls = pEnv->FindClass("com/primesense/NiteControls/HandPointContext");
	jmethodID HandPointContextCtor = pEnv->GetMethodID(HandPointContextCls, "<init>", "(ILorg/openni/Point3D;FF)V");

	jobject handPosition = CreateObjectFromPoint3D(pEnv, &pHand->ptPosition);

	return pEnv->NewObject(HandPointContextCls, HandPointContextCtor, pHand->nID, handPosition, pHand->fTime, pHand->fConfidence);
}

void SetOutArgPoint3DValue(JNIEnv* env, jobject p, const XnPoint3D* pPoint)
{
	SetOutArgObjectValue(env, p, CreateObjectFromPoint3D(env, pPoint));
}

//---------------------------------------------------------------------------
// Types
//---------------------------------------------------------------------------
class JavaString
{
public:
	JavaString(JNIEnv *env, jstring str) : m_env(env), m_str(str), m_utfString(env->GetStringUTFChars(str, NULL)) 
	{
	}

	~JavaString()
	{
		m_env->ReleaseStringUTFChars(m_str, m_utfString);
	}

	operator const char*()
	{
		return m_utfString;
	}

private:
	JNIEnv* m_env;
	jstring m_str;
	const char* m_utfString;
};

class JNIEnvSupplier
{
public:
	JNIEnvSupplier() : m_pEnv(NULL), m_bShouldDetach(FALSE)
	{
		if (JNI_EDETACHED == g_pVM->GetEnv((void**)&m_pEnv, JNI_VERSION_1_2))
		{
			g_pVM->AttachCurrentThread((void**)&m_pEnv, NULL);
			m_bShouldDetach = TRUE;
		}
	}

	~JNIEnvSupplier()
	{
		if (m_bShouldDetach)
		{
			g_pVM->DetachCurrentThread();
		}
	}

	JNIEnv* GetEnv() { return m_pEnv; }

private:
	JNIEnv* m_pEnv;
	XnBool m_bShouldDetach;
};

//---------------------------------------------------------------------------
// Callbacks
//---------------------------------------------------------------------------
class CallbackCookie
{
public:
	CallbackCookie(JNIEnv *env, jobject obj, jstring cb, const char* sig) :
	  hCallback(_hCallback)
	{
		_cls = env->GetObjectClass(obj);
		_mid = env->GetMethodID(_cls, JavaString(env, cb), sig);
		_env = env;
		_obj = env->NewGlobalRef(obj);

		jboolean refSuccess = env->IsSameObject(obj, _obj);
		if (!refSuccess)
		{
			printf("Reference failure!\n");
		}
	}

	~CallbackCookie()
	{
		_env->DeleteGlobalRef(_obj);
	}

	inline JNIEnv* GetEnv() {return _env;}
	inline jclass GetClass() {return _cls;}
	inline jobject GetObject() {return _obj;}
	inline jmethodID GetMethodID() {return _mid;}
	inline XnCallbackHandle GetHandle() {return _hCallback;}
private:
	JNIEnv* _env;
	jclass _cls; // need to store class so that method ID is still valid
	jobject _obj;
	jmethodID _mid;
	XnCallbackHandle _hCallback;
public:
	XnCallbackHandle& hCallback;
};


//---------------------------------------------------------------------------
// Broadcaster
//---------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvBroadcaster_1Create
  (JNIEnv* env, jclass cls, jstring name)
{
	JavaString jName(env, name);
	return (jlong)xnvBroadcaster_Create(jName);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvBroadcaster_1Destroy
  (JNIEnv* env, jclass cls, jlong broadcaster)
{
	return xnvBroadcaster_Destroy((XnVObjectID)broadcaster);
}
//---------------------------------------------------------------------------
// MessageGenerator
//---------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvMessageGenerator_1AddListener
  (JNIEnv* env, jclass cls, jlong messageGenerator, jlong messageListener)
{
	return xnvMessageGenerator_AddListener((XnVObjectID)messageGenerator, (XnVObjectID)messageListener);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvMessageGenerator_1RemoveListener
  (JNIEnv* env, jclass cls, jlong messageGenerator, jlong messageListener)
{
	return xnvMessageGenerator_RemoveListener((XnVObjectID)messageGenerator, (XnVObjectID)messageListener);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvMessageGenerator_1Generate
  (JNIEnv *env, jclass cls, jlong messageGenerator, jlong message)
{
	return xnvMessageGenerator_Generate((XnVObjectID)messageGenerator, (void*)message);
}
//---------------------------------------------------------------------------
// MessageListener
//---------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvMessageListener_1Update
  (JNIEnv *env, jclass cls, jlong messageListener, jlong message)
{
	return xnvMessageListener_Update((XnVObjectID)messageListener, (void*)message);
}
/// MessageListener::Activate
void XN_CALLBACK_TYPE MessageListenerActivateHandler(void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID());
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvMessageListener_1RegisterActivate
(JNIEnv *env, jclass cls, jlong messageListener, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "()V");
	XnStatus rc = xnvMessageListener_RegisterActivate((XnVObjectID)messageListener, MessageListenerActivateHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvMessageListener_1UnregisterActivate
(JNIEnv *env, jclass cls, jlong messageListener, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvMessageListener_UnregisterActivate((XnVObjectID)messageListener, pCookie->hCallback);
	delete pCookie;

	return rc;
}

/// MessageListener::Deactivate
void XN_CALLBACK_TYPE MessageListenerDeactivateHandler(void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID());
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvMessageListener_1RegisterDeactivate
(JNIEnv *env, jclass cls, jlong messageListener, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "()V");
	XnStatus rc = xnvMessageListener_RegisterDeactivate((XnVObjectID)messageListener, MessageListenerDeactivateHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvMessageListener_1UnregisterDeactivate
(JNIEnv *env, jclass cls, jlong messageListener, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvMessageListener_UnregisterDeactivate((XnVObjectID)messageListener, pCookie->hCallback);
	delete pCookie;

	return rc;
}

/// MessageListener::Update
void XN_CALLBACK_TYPE MessageListenerUpdateHandler(void* message, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), (jlong)message);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvMessageListener_1RegisterUpdate
(JNIEnv *env, jclass cls, jlong messageListener, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(J)V");
	XnStatus rc = xnvMessageListener_RegisterUpdate((XnVObjectID)messageListener, MessageListenerUpdateHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvMessageListener_1UnregisterUpdate
(JNIEnv *env, jclass cls, jlong messageListener, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvMessageListener_UnregisterUpdate((XnVObjectID)messageListener, pCookie->hCallback);
	delete pCookie;

	return rc;
}

//---------------------------------------------------------------------------
// FlowRouter
//---------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvFlowRouter_1Create
  (JNIEnv* env, jclass cls, jstring name)
{
	JavaString jName(env, name);
	return (jlong)xnvFlowRouter_Create(jName);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvFlowRouter_1Destroy
  (JNIEnv* env, jclass cls, jlong flowRouter)
{
	return xnvFlowRouter_Destroy((XnVObjectID)flowRouter);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvFlowRouter_1SetActive
  (JNIEnv* env, jclass cls, jlong flowRouter, jlong messageListener)
{
	return xnvFlowRouter_SetActive((XnVObjectID)flowRouter, (XnVObjectID)messageListener);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvFlowRouter_1GetActive
  (JNIEnv* env, jclass cls, jlong flowRouter, jobject pMessageListener)
{
	XnVObjectID inListener = 0;
	XnStatus rc = xnvFlowRouter_GetActive((XnVObjectID)flowRouter, &inListener);
	SetOutArgObjectIDValue(env, pMessageListener, inListener);
	return rc;
}
//---------------------------------------------------------------------------
// PointControl
//---------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1Create
  (JNIEnv* env, jclass cls, jstring name)
{
	JavaString jName(env, name);
	return (jlong)xnvPointControl_Create(jName);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1Destroy
  (JNIEnv* env, jclass cls, jlong pointControl)
{
	return xnvPointControl_Destroy((XnVObjectID)pointControl);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1GetPrimaryID
(JNIEnv* env, jclass cls, jlong pointControl, jobject pPrimaryID)
{
	XnUInt32 nPrimaryID = 0;
	XnStatus rc = xnvPointControl_GetPrimaryID((XnVObjectID)pointControl, &nPrimaryID);
	SetOutArgIntValue(env, pPrimaryID, nPrimaryID);
	return rc;
}

/// PointControl::PointCreate
void XN_CALLBACK_TYPE PointControlPointCreateHandler(const XnVHandPointContext* pHand, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), CreateObjectFromHandPointContext(JNIEnvSupplier().GetEnv(), pHand));
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1RegisterPointCreate
(JNIEnv *env, jclass cls, jlong pointControl, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(Lcom/primesense/NiteControls/HandPointContext;)V");
	XnStatus rc = xnvPointControl_RegisterPointCreate((XnVObjectID)pointControl, PointControlPointCreateHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1UnregisterPointCreate
(JNIEnv *env, jclass cls, jlong pointControl, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPointControl_UnregisterPointCreate((XnVObjectID)pointControl, pCookie->hCallback);
	delete pCookie;

	return rc;
}

/// PointControl::PointUpdate
void XN_CALLBACK_TYPE PointControlPointUpdateHandler(const XnVHandPointContext* pHand, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), CreateObjectFromHandPointContext(JNIEnvSupplier().GetEnv(), pHand));
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1RegisterPointUpdate
(JNIEnv *env, jclass cls, jlong pointControl, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(Lcom/primesense/NiteControls/HandPointContext;)V");
	XnStatus rc = xnvPointControl_RegisterPointUpdate((XnVObjectID)pointControl, PointControlPointUpdateHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1UnregisterPointUpdate
(JNIEnv *env, jclass cls, jlong pointControl, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPointControl_UnregisterPointUpdate((XnVObjectID)pointControl, pCookie->hCallback);
	delete pCookie;

	return rc;
}

/// PointControl::PointDestroy
void XN_CALLBACK_TYPE PointControlPointDestroyHandler(XnUInt32 id, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), id);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1RegisterPointDestroy
(JNIEnv *env, jclass cls, jlong pointControl, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(I)V");
	XnStatus rc = xnvPointControl_RegisterPointDestroy((XnVObjectID)pointControl, PointControlPointDestroyHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1UnregisterPointDestroy
(JNIEnv *env, jclass cls, jlong pointControl, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPointControl_UnregisterPointDestroy((XnVObjectID)pointControl, pCookie->hCallback);
	delete pCookie;

	return rc;
}

/// PointControl::PrimaryPointCreate

void XN_CALLBACK_TYPE PointControlPrimaryPointCreateHandler(const XnVHandPointContext* pHand, const XnPoint3D* pFocus, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	jobject focusPoint = CreateObjectFromPoint3D(JNIEnvSupplier().GetEnv(), pFocus);
	jobject handPointContext = CreateObjectFromHandPointContext(JNIEnvSupplier().GetEnv(), pHand);

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), handPointContext, focusPoint);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1RegisterPrimaryPointCreate
(JNIEnv *env, jclass cls, jlong pointControl, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(Lcom/primesense/NiteControls/HandPointContext;Lorg/openni/Point3D;)V");
	XnStatus rc = xnvPointControl_RegisterPrimaryPointCreate((XnVObjectID)pointControl, PointControlPrimaryPointCreateHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1UnregisterPrimaryPointCreate
(JNIEnv *env, jclass cls, jlong pointControl, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPointControl_UnregisterPrimaryPointCreate((XnVObjectID)pointControl, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// PointControl::PrimaryPointUpdate
void XN_CALLBACK_TYPE PointControlPrimaryPointUpdateHandler(const XnVHandPointContext* pHand, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), CreateObjectFromHandPointContext(JNIEnvSupplier().GetEnv(), pHand));
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1RegisterPrimaryPointUpdate
(JNIEnv *env, jclass cls, jlong pointControl, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(Lcom/primesense/NiteControls/HandPointContext;)V");
	XnStatus rc = xnvPointControl_RegisterPrimaryPointUpdate((XnVObjectID)pointControl, PointControlPrimaryPointUpdateHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1UnregisterPrimaryPointUpdate
(JNIEnv *env, jclass cls, jlong pointControl, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPointControl_UnregisterPrimaryPointUpdate((XnVObjectID)pointControl, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// PointControl::PrimaryPointReplace
void XN_CALLBACK_TYPE PointControlPrimaryPointReplaceHandler(XnUInt32 nReplaced, const XnVHandPointContext* pHand, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), CreateObjectFromHandPointContext(JNIEnvSupplier().GetEnv(), pHand), nReplaced);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1RegisterPrimaryPointReplace
(JNIEnv *env, jclass cls, jlong pointControl, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(Lcom/primesense/NiteControls/HandPointContext;I)V");
	XnStatus rc = xnvPointControl_RegisterPrimaryPointReplace((XnVObjectID)pointControl, PointControlPrimaryPointReplaceHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1UnregisterPrimaryPointReplace
(JNIEnv *env, jclass cls, jlong pointControl, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPointControl_UnregisterPrimaryPointReplace((XnVObjectID)pointControl, pCookie->hCallback);
	delete pCookie;

	return rc;
}

/// PointControl::PrimaryPointDestroy
void XN_CALLBACK_TYPE PointControlPrimaryPointDestroyHandler(XnUInt32 id, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), id);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1RegisterPrimaryPointDestroy
(JNIEnv *env, jclass cls, jlong pointControl, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(I)V");
	XnStatus rc = xnvPointControl_RegisterPrimaryPointDestroy((XnVObjectID)pointControl, PointControlPrimaryPointDestroyHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1UnregisterPrimaryPointDestroy
(JNIEnv *env, jclass cls, jlong pointControl, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPointControl_UnregisterPrimaryPointDestroy((XnVObjectID)pointControl, pCookie->hCallback);
	delete pCookie;

	return rc;
}

/// PointControl::NoPoints
void XN_CALLBACK_TYPE PointControlNoPointsHandler(void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID());
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1RegisterNoPoints
(JNIEnv *env, jclass cls, jlong pointControl, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "()V");
	XnStatus rc = xnvPointControl_RegisterNoPoints((XnVObjectID)pointControl, PointControlNoPointsHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointControl_1UnregisterNoPoints
(JNIEnv *env, jclass cls, jlong pointControl, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPointControl_UnregisterNoPoints((XnVObjectID)pointControl, pCookie->hCallback);
	delete pCookie;

	return rc;
}

//---------------------------------------------------------------------------
// PushDetector
//---------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1Create
  (JNIEnv* env, jclass cls, jstring name)
{
	JavaString jName(env, name);
	return (jlong)xnvPushDetector_Create(jName);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1Destroy
  (JNIEnv* env, jclass cls, jlong pushDetector)
{
	return xnvPushDetector_Destroy((XnVObjectID)pushDetector);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1Reset
(JNIEnv *, jclass, jlong pushDetector)
{
	return xnvPushDetector_Reset((XnVObjectID)pushDetector);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1SetPushImmediateMinimumVelocity
(JNIEnv* env, jclass cls, jlong pushDetector, jfloat velocity)
{
	return xnvPushDetector_SetPushImmediateMinimumVelocity((XnVObjectID)pushDetector, velocity);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1SetPushImmediateDuration
(JNIEnv* env, jclass cls, jlong pushDetector, jint duration)
{
	return xnvPushDetector_SetPushImmediateDuration((XnVObjectID)pushDetector, duration);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1SetPushImmediateOffset
(JNIEnv* env, jclass cls, jlong pushDetector, jint offset)
{
	return xnvPushDetector_SetPushImmediateOffset((XnVObjectID)pushDetector, offset);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1SetPushPreviousMinimumVelocity
(JNIEnv* env, jclass cls, jlong pushDetector, jfloat velocity)
{
	return xnvPushDetector_SetPushPreviousMinimumVelocity((XnVObjectID)pushDetector, velocity);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1SetPushPreviousDuration
(JNIEnv* env, jclass cls, jlong pushDetector, jint duration)
{
	return xnvPushDetector_SetPushPreviousDuration((XnVObjectID)pushDetector, duration);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1SetPushPreviousOffset
(JNIEnv* env, jclass cls, jlong pushDetector, jint offset)
{
	return xnvPushDetector_SetPushPreviousOffset((XnVObjectID)pushDetector, offset);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1SetPushMaximumAngleBetweenImmediateAndZ
(JNIEnv* env, jclass cls, jlong pushDetector, jfloat angle)
{
	return xnvPushDetector_SetPushMaximumAngleBetweenImmediateAndZ((XnVObjectID)pushDetector, angle);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1SetPushMinimumAngleBetweenImmediateAndPrevious
(JNIEnv* env, jclass cls, jlong pushDetector, jfloat angle)
{
	return xnvPushDetector_SetPushMinimumAngleBetweenImmediateAndPrevious((XnVObjectID)pushDetector, angle);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1SetStableMaximumVelocity
(JNIEnv* env, jclass cls, jlong pushDetector, jfloat velocity)
{
	return xnvPushDetector_SetStableMaximumVelocity((XnVObjectID)pushDetector, velocity);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1SetStableDuration
(JNIEnv* env, jclass cls, jlong pushDetector, jint duration)
{
	return xnvPushDetector_SetStableDuration((XnVObjectID)pushDetector, duration);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1GetPushImmediateMinimumVelocity
(JNIEnv* env, jclass cls, jlong pushDetector, jobject pVelocity)
{
	XnFloat fVelocity = 0;
	XnStatus rc = xnvPushDetector_GetPushImmediateMinimumVelocity((XnVObjectID)pushDetector, &fVelocity);
	SetOutArgFloatValue(env, pVelocity, fVelocity);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1GetPushImmediateDuration
(JNIEnv* env, jclass cls, jlong pushDetector, jobject pDuration)
{
	XnUInt32 nDuration = 0;
	XnStatus rc = xnvPushDetector_GetPushImmediateDuration((XnVObjectID)pushDetector, &nDuration);
	SetOutArgIntValue(env, pDuration, nDuration);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1GetPushImmediateOffset
(JNIEnv* env, jclass cls, jlong pushDetector, jobject pOffset)
{
	XnUInt32 nOffset = 0;
	XnStatus rc = xnvPushDetector_GetPushImmediateOffset((XnVObjectID)pushDetector, &nOffset);
	SetOutArgIntValue(env, pOffset, nOffset);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1GetPushPreviousMinimumVelocity
(JNIEnv* env, jclass cls, jlong pushDetector, jobject pVelocity)
{
	XnFloat fVelocity = 0;
	XnStatus rc = xnvPushDetector_GetPushPreviousMinimumVelocity((XnVObjectID)pushDetector, &fVelocity);
	SetOutArgFloatValue(env, pVelocity, fVelocity);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1GetPushPreviousDuration
(JNIEnv* env, jclass cls, jlong pushDetector, jobject pDuration)
{
	XnUInt32 nDuration = 0;
	XnStatus rc = xnvPushDetector_GetPushPreviousDuration((XnVObjectID)pushDetector, &nDuration);
	SetOutArgIntValue(env, pDuration, nDuration);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1GetPushPreviousOffset
(JNIEnv* env, jclass cls, jlong pushDetector, jobject pOffset)
{
	XnUInt32 nOffset = 0;
	XnStatus rc = xnvPushDetector_GetPushPreviousOffset((XnVObjectID)pushDetector, &nOffset);
	SetOutArgIntValue(env, pOffset, nOffset);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1GetPushMaximumAngleBetweenImmediateAndZ
(JNIEnv* env, jclass cls, jlong pushDetector, jobject pAngle)
{
	XnFloat fAngle = 0;
	XnStatus rc = xnvPushDetector_GetPushMaximumAngleBetweenImmediateAndZ((XnVObjectID)pushDetector, &fAngle);
	SetOutArgFloatValue(env, pAngle, fAngle);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1GetPushMinimumAngleBetweenImmediateAndPrevious
(JNIEnv* env, jclass cls, jlong pushDetector, jobject pAngle)
{
	XnFloat fAngle = 0;
	XnStatus rc = xnvPushDetector_GetPushMinimumAngleBetweenImmediateAndPrevious((XnVObjectID)pushDetector, &fAngle);
	SetOutArgFloatValue(env, pAngle, fAngle);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1GetStableMaximumVelocity
(JNIEnv* env, jclass cls, jlong pushDetector, jobject pVelocity)
{
	XnFloat fVelocity = 0;
	XnStatus rc = xnvPushDetector_GetStableMaximumVelocity((XnVObjectID)pushDetector, &fVelocity);
	SetOutArgFloatValue(env, pVelocity, fVelocity);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1GetStableDuration
(JNIEnv* env, jclass cls, jlong pushDetector, jobject pDuration)
{
	XnUInt32 nDuration = 0;
	XnStatus rc = xnvPushDetector_GetStableDuration((XnVObjectID)pushDetector, &nDuration);
	SetOutArgIntValue(env, pDuration, nDuration);
	return rc;
}

/// PushDetector::Push
void XN_CALLBACK_TYPE PushDetectorPushHandler(XnFloat velocity, XnFloat angle, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), velocity, angle);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1RegisterPush
(JNIEnv *env, jclass cls, jlong pushDetector, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(FF)V");
	XnStatus rc = xnvPushDetector_RegisterPush((XnVObjectID)pushDetector, PushDetectorPushHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1UnregisterPush
(JNIEnv *env, jclass cls, jlong pushDetector, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPushDetector_UnregisterPush((XnVObjectID)pushDetector, pCookie->hCallback);
	delete pCookie;

	return rc;
}

/// PushDetector::Stabilized
void XN_CALLBACK_TYPE PushDetectorStabilizedHandler(XnFloat velocity, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), velocity);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1RegisterStabilized
(JNIEnv *env, jclass cls, jlong pushDetector, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(F)V");
	XnStatus rc = xnvPushDetector_RegisterStabilized((XnVObjectID)pushDetector, PushDetectorStabilizedHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPushDetector_1UnregisterStabilized
(JNIEnv *env, jclass cls, jlong pushDetector, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPushDetector_UnregisterStabilized((XnVObjectID)pushDetector, pCookie->hCallback);
	delete pCookie;

	return rc;
}
//---------------------------------------------------------------------------
// SelectableSlider1D
//---------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1Create
  (JNIEnv* env, jclass cls, jint count, jint axis, jboolean recenter, jfloat sliderSize, jfloat firstValue, jstring name)
{
	JavaString jName(env, name);
	return (jlong)xnvSelectableSlider1D_Create(count, (XnVAxis)axis, recenter, sliderSize, firstValue, jName);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1Destroy
  (JNIEnv* env, jclass cls, jlong ss1)
{
	return xnvSelectableSlider1D_Destroy((XnVObjectID)ss1);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1Reposition
  (JNIEnv* env, jclass cls, jlong ss1, jfloat x, jfloat y, jfloat z)
{
	XnPoint3D point = xnCreatePoint3D(x, y, z);
	return xnvSelectableSlider1D_Reposition((XnVObjectID)ss1, &point);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1GetCenter
  (JNIEnv* env, jclass cls, jlong ss1, jobject pt)
{
	XnPoint3D point;
	XnStatus rc = xnvSelectableSlider1D_GetCenter((XnVObjectID)ss1, &point);
	SetOutArgPoint3DValue(env, pt, &point);
	return rc;
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1SetItemCount
(JNIEnv* env, jclass cls, jlong ss1, jint items)
{
	return xnvSelectableSlider1D_SetItemCount((XnVObjectID)ss1, items);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1GetItemCount
(JNIEnv* env, jclass cls, jlong ss1, jobject pItems)
{
	XnUInt32 nItems = 0;
	XnStatus rc = xnvSelectableSlider1D_GetItemCount((XnVObjectID)ss1, &nItems);
	SetOutArgIntValue(env, pItems, nItems);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1SetSliderSize
(JNIEnv* env, jclass cls, jlong ss1, jfloat slider)
{
	return xnvSelectableSlider1D_SetSliderSize((XnVObjectID)ss1, slider);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1GetSliderSize
(JNIEnv* env, jclass cls, jlong ss1, jobject pSliderSize)
{
	XnFloat fSliderSize = 0;
	XnStatus rc = xnvSelectableSlider1D_GetSliderSize((XnVObjectID)ss1, &fSliderSize);
	SetOutArgFloatValue(env, pSliderSize, fSliderSize);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1SetValueChangeOnOffAxis
(JNIEnv* env, jclass cls, jlong ss1, jboolean report)
{
	return xnvSelectableSlider1D_SetValueChangeOnOffAxis((XnVObjectID)ss1, report);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1GetValueChangeOnOffAxis
(JNIEnv* env, jclass cls, jlong ss1, jobject pReport)
{
	XnBool bReport;
	XnStatus rc = xnvSelectableSlider1D_GetValueChangeOnOffAxis((XnVObjectID)ss1, &bReport);
	SetOutArgIntValue(env, pReport, bReport);
	return rc;
}

/// SelectableSlider1D::ItemHover
void XN_CALLBACK_TYPE SelectableSlider1DItemHoverHandler(XnInt32 item, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), item);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1RegisterItemHover
(JNIEnv *env, jclass cls, jlong ss1, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(I)V");
	XnStatus rc = xnvSelectableSlider1D_RegisterItemHover((XnVObjectID)ss1, SelectableSlider1DItemHoverHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1UnregisterItemHover
(JNIEnv *env, jclass cls, jlong ss1, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSelectableSlider1D_UnregisterItemHover((XnVObjectID)ss1, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SelectableSlider1D::ItemSelect
void XN_CALLBACK_TYPE SelectableSlider1DItemSelectHandler(XnInt32 item, XnVDirection dir, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), item, dir);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1RegisterItemSelect
(JNIEnv *env, jclass cls, jlong ss1, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(II)V");
	XnStatus rc = xnvSelectableSlider1D_RegisterItemSelect((XnVObjectID)ss1, SelectableSlider1DItemSelectHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1UnregisterItemSelect
(JNIEnv *env, jclass cls, jlong ss1, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSelectableSlider1D_UnregisterItemSelect((XnVObjectID)ss1, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SelectableSlider1D::ValueChange
void XN_CALLBACK_TYPE SelectableSlider1DValueChangeHandler(XnFloat value, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), value);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1RegisterValueChange
(JNIEnv *env, jclass cls, jlong ss1, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(F)V");
	XnStatus rc = xnvSelectableSlider1D_RegisterValueChange((XnVObjectID)ss1, SelectableSlider1DValueChangeHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1UnregisterValueChange
(JNIEnv *env, jclass cls, jlong ss1, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSelectableSlider1D_UnregisterValueChange((XnVObjectID)ss1, pCookie->hCallback);
	delete pCookie;

	return rc;
}

/// SelectableSlider1D::OffAxisMovement
void XN_CALLBACK_TYPE SelectableSlider1DOffAxisMovementHandler(XnVDirection eDir, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), (XnUInt32)eDir);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1RegisterOffAxisMovement
(JNIEnv *env, jclass cls, jlong ss1, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(I)V");
	XnStatus rc = xnvSelectableSlider1D_RegisterOffAxisMovement((XnVObjectID)ss1, SelectableSlider1DOffAxisMovementHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider1D_1UnregisterOffAxisMovement
(JNIEnv *env, jclass cls, jlong ss1, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSelectableSlider1D_UnregisterOffAxisMovement((XnVObjectID)ss1, pCookie->hCallback);
	delete pCookie;

	return rc;
}

//---------------------------------------------------------------------------
// SelectableSlider2D
//---------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1Create
  (JNIEnv* env, jclass cls, jint countX, jint countY, jfloat sliderSizeX, jfloat sliderSizeY, jstring name)
{
	JavaString jName(env, name);
	return (jlong)xnvSelectableSlider2D_Create(countX, countY, sliderSizeX, sliderSizeY, jName);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1Destroy
  (JNIEnv* env, jclass cls, jlong ss2)
{
	return xnvSelectableSlider2D_Destroy((XnVObjectID)ss2);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1Reposition
  (JNIEnv* env, jclass cls, jlong ss2, jfloat x, jfloat y, jfloat z)
{
	XnPoint3D point = xnCreatePoint3D(x, y, z);
	return xnvSelectableSlider2D_Reposition((XnVObjectID)ss2, &point);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1GetCenter
  (JNIEnv* env, jclass cls, jlong ss2, jobject pt)
{
	XnPoint3D point;
	XnStatus rc = xnvSelectableSlider2D_GetCenter((XnVObjectID)ss2, &point);
	SetOutArgPoint3DValue(env, pt, &point);
	return rc;
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1GetItemXCount
(JNIEnv* env, jclass cls, jlong ss2, jobject pItems)
{
	XnUInt32 nItems = 0;
	XnStatus rc = xnvSelectableSlider2D_GetItemXCount((XnVObjectID)ss2, &nItems);
	SetOutArgIntValue(env, pItems, nItems);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1SetItemXCount
(JNIEnv* env, jclass cls, jlong ss2, jint items)
{
	return xnvSelectableSlider2D_SetItemXCount((XnVObjectID)ss2, items);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1GetItemYCount
(JNIEnv* env, jclass cls, jlong ss2, jobject pItems)
{
	XnUInt32 nItems = 0;
	XnStatus rc = xnvSelectableSlider2D_GetItemYCount((XnVObjectID)ss2, &nItems);
	SetOutArgIntValue(env, pItems, nItems);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1SetItemYCount
(JNIEnv* env, jclass cls, jlong ss2, jint items)
{
	return xnvSelectableSlider2D_SetItemYCount((XnVObjectID)ss2, items);
}
// JNIEXPORT void JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1SetItemCount
// (JNIEnv* env, jclass cls, jlong ss2, jint, jint);
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1GetSliderXSize
(JNIEnv* env, jclass cls, jlong ss2, jobject pSliderSize)
{
	XnFloat fSliderSize = 0;
	XnFloat fDummy;
	XnStatus rc = xnvSelectableSlider2D_GetSliderSize((XnVObjectID)ss2, &fSliderSize, &fDummy);
	SetOutArgFloatValue(env, pSliderSize, fSliderSize);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1GetSliderYSize
(JNIEnv* env, jclass cls, jlong ss2, jobject pSliderSize)
{
	XnFloat fSliderSize = 0;
	XnFloat fDummy;
	XnStatus rc = xnvSelectableSlider2D_GetSliderSize((XnVObjectID)ss2, &fDummy, &fSliderSize);
	SetOutArgFloatValue(env, pSliderSize, fSliderSize);
	return rc;
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1SetSliderSize
(JNIEnv* env, jclass cls, jlong ss2, jfloat sliderXSize, jfloat sliderYSize)
{
	return xnvSelectableSlider2D_SetSliderSize((XnVObjectID)ss2, sliderXSize, sliderYSize);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1GetValueChangeOnOffAxis
(JNIEnv* env, jclass cls, jlong ss2, jobject pReport)
{
	XnBool bReport = FALSE;
	XnStatus rc = xnvSelectableSlider2D_GetValueChangeOnOffAxis((XnVObjectID)ss2, &bReport);
	SetOutArgBooleanValue(env, pReport, bReport);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1SetValueChangeOnOffAxis
(JNIEnv* env, jclass cls, jlong ss2, jboolean report)
{
	return xnvSelectableSlider2D_SetValueChangeOnOffAxis((XnVObjectID)ss2, report);
}

/// SelectableSlider2D::ItemHover
void XN_CALLBACK_TYPE SelectableSlider2DItemHoverHandler(XnInt32 itemX, XnInt32 itemY, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), itemX, itemY);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1RegisterItemHover
(JNIEnv *env, jclass cls, jlong ss2, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(II)V");
	XnStatus rc = xnvSelectableSlider2D_RegisterItemHover((XnVObjectID)ss2, SelectableSlider2DItemHoverHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1UnregisterItemHover
(JNIEnv *env, jclass cls, jlong ss2, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSelectableSlider2D_UnregisterItemHover((XnVObjectID)ss2, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SelectableSlider2D::ItemSelect
void XN_CALLBACK_TYPE SelectableSlider2DItemSelectHandler(XnInt32 itemX, XnInt32 itemY, XnVDirection dir, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), itemX, itemY, dir);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1RegisterItemSelect
(JNIEnv *env, jclass cls, jlong ss2, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(III)V");
	XnStatus rc = xnvSelectableSlider2D_RegisterItemSelect((XnVObjectID)ss2, SelectableSlider2DItemSelectHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1UnregisterItemSelect
(JNIEnv *env, jclass cls, jlong ss2, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSelectableSlider2D_UnregisterItemSelect((XnVObjectID)ss2, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SelectableSlider2D::ValueChange
void XN_CALLBACK_TYPE SelectableSlider2DValueChangeHandler(XnFloat valueX, XnFloat valueY, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), valueX, valueY);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1RegisterValueChange
(JNIEnv *env, jclass cls, jlong ss2, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(FF)V");
	XnStatus rc = xnvSelectableSlider2D_RegisterValueChange((XnVObjectID)ss2, SelectableSlider2DValueChangeHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1UnregisterValueChange
(JNIEnv *env, jclass cls, jlong ss2, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSelectableSlider2D_UnregisterValueChange((XnVObjectID)ss2, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SelectableSlider2D::OffAxisMovement
void XN_CALLBACK_TYPE SelectableSlider2DOffAxisMovementHandler(XnVDirection eDir, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), (XnUInt32)eDir);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1RegisterOffAxisMovement
(JNIEnv *env, jclass cls, jlong ss2, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(I)V");
	XnStatus rc = xnvSelectableSlider2D_RegisterOffAxisMovement((XnVObjectID)ss2, SelectableSlider2DOffAxisMovementHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSelectableSlider2D_1UnregisterOffAxisMovement
(JNIEnv *env, jclass cls, jlong ss2, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSelectableSlider2D_UnregisterOffAxisMovement((XnVObjectID)ss2, pCookie->hCallback);
	delete pCookie;

	return rc;
}

//---------------------------------------------------------------------------
// SessionManager
//---------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionManager_1Create
  (JNIEnv* env, jclass cls, jstring name)
{
	JavaString jName(env, name);
	return (jlong)xnvSessionManager_Create(jName);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionManager_1Initialize
  (JNIEnv* env, jclass cls, jlong sessionManager, jlong context, jstring focus)
{
	JavaString jFocus(env, focus);
	return xnvSessionManager_Initialize((XnVObjectID)sessionManager, (XnContext*)context, jFocus, NULL, NULL);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionManager_1Destroy
  (JNIEnv* env, jclass cls, jlong sessionManager)
{
	return xnvSessionManager_Destroy((XnVObjectID)sessionManager);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionManager_1Update
  (JNIEnv* env, jclass cls, jlong sessionManager, jlong context)
{
	return xnvSessionManager_Update((XnVObjectID)sessionManager, (XnContext*)context);
}


JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionManager_1AddGestureByContext
(JNIEnv* env, jclass cls, jlong sessionManager, jlong context, jstring name, jobject pGestureID)
{
	JavaString jName(env, name);
	XnUInt32 nGestureID = 0;
	XnStatus rc = xnvSessionManager_AddGestureByContext((XnVObjectID)sessionManager, (XnContext*)context, jName, &nGestureID);
	SetOutArgIntValue(env, pGestureID, nGestureID);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionManager_1RemoveGesture
(JNIEnv* env, jclass cls, jlong sessionManager, jint gestureID)
{
	return xnvSessionManager_RemoveGesture((XnVObjectID)sessionManager, gestureID);
}

//---------------------------------------------------------------------------
// SessionGenerator
//---------------------------------------------------------------------------

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1LosePoint
(JNIEnv *env, jclass cls, jlong sessionGenerator, jint pointId)
{
	return xnvSessionGenerator_LosePoint((XnVObjectID)sessionGenerator, pointId);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1LosePoints
(JNIEnv* env, jclass cls, jlong sessionGenerator)
{
	return xnvSessionGenerator_LosePoints((XnVObjectID)sessionGenerator);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1EndSession
(JNIEnv* env, jclass cls, jlong sessionGenerator)
{
	return xnvSessionGenerator_EndSession((XnVObjectID)sessionGenerator);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1ForceSession
(JNIEnv* env, jclass cls, jlong sessionGenerator, jfloat x, jfloat y, jfloat z)
{
	XnPoint3D point = xnCreatePoint3D(x, y, z);
	return xnvSessionGenerator_ForceSession((XnVObjectID)sessionGenerator, &point);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1TrackPoint
(JNIEnv* env, jclass cls, jlong sessionGenerator, jfloat x, jfloat y, jfloat z)
{
	XnPoint3D point = xnCreatePoint3D(x, y, z);
	return xnvSessionGenerator_TrackPoint((XnVObjectID)sessionGenerator, &point);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1IsInSession
(JNIEnv* env, jclass cls, jlong sessionGenerator, jobject pInSession)
{
	XnBool bInSession = FALSE;
	XnStatus rc = xnvSessionGenerator_IsInSession((XnVObjectID)sessionGenerator, &bInSession);
	SetOutArgBooleanValue(env, pInSession, bInSession);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1GetFocusPoint
(JNIEnv* env, jclass cls, jlong sessionGenerator, jobject pFocus)
{
	XnPoint3D focus;
	XnStatus rc = xnvSessionGenerator_GetFocusPoint((XnVObjectID)sessionGenerator, &focus);
	SetOutArgPoint3DValue(env, pFocus, &focus);
	return rc;
}

/// SessionGenerator::SessionStart
void XN_CALLBACK_TYPE SessionGeneratorSessionStartHandler(const XnPoint3D* pPosition, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), CreateObjectFromPoint3D(JNIEnvSupplier().GetEnv(), pPosition));
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1RegisterToSessionStart
(JNIEnv *env, jclass cls, jlong sessionGenerator, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(Lorg/openni/Point3D;)V");
	XnStatus rc = xnvSessionGenerator_RegisterToSessionStart((XnVObjectID)sessionGenerator, SessionGeneratorSessionStartHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1UnregisterFromSessionStart
(JNIEnv *env, jclass cls, jlong sessionGenerator, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSessionGenerator_UnregisterFromSessionStart((XnVObjectID)sessionGenerator, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SessionGenerator::SessionEnd
void XN_CALLBACK_TYPE SessionGeneratorSessionEndHandler(void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID());
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1RegisterToSessionEnd
(JNIEnv *env, jclass cls, jlong sessionGenerator, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "()V");
	XnStatus rc = xnvSessionGenerator_RegisterToSessionEnd((XnVObjectID)sessionGenerator, SessionGeneratorSessionEndHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1UnregisterFromSessionEnd
(JNIEnv *env, jclass cls, jlong sessionGenerator, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSessionGenerator_UnregisterFromSessionEnd((XnVObjectID)sessionGenerator, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SessionGenerator::SessionFocusProgress
void XN_CALLBACK_TYPE SessionGeneratorSessionFocusProgressHandler(const XnChar* strFocus, const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), JNIEnvSupplier().GetEnv()->NewStringUTF(strFocus), CreateObjectFromPoint3D(JNIEnvSupplier().GetEnv(), pPosition), fProgress);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1RegisterToSessionFocusProgress
(JNIEnv *env, jclass cls, jlong sessionGenerator, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(Ljava/lang/String;Lorg/openni/Point3D;F)V");
	XnStatus rc = xnvSessionGenerator_RegisterToSessionFocusProgress((XnVObjectID)sessionGenerator, SessionGeneratorSessionFocusProgressHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSessionGenerator_1UnregisterFromSessionFocusProgress
(JNIEnv *env, jclass cls, jlong sessionGenerator, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSessionGenerator_UnregisterFromSessionFocusProgress((XnVObjectID)sessionGenerator, pCookie->hCallback);
	delete pCookie;

	return rc;
}

//---------------------------------------------------------------------------
// SteadyDetector
//---------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1Create
  (JNIEnv* env, jclass cls, jint cooldown, jint duration, jfloat stdDev, jstring name)
{
	JavaString jName(env, name);
	return (jlong)xnvSteadyDetector_Create(cooldown, duration, stdDev, jName);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1Destroy
  (JNIEnv* env, jclass cls, jlong steadyDetector)
{
	return xnvSteadyDetector_Destroy((XnVObjectID)steadyDetector);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1Reset
(JNIEnv* env, jclass cls, jlong steadyDetector)
{
	return xnvSteadyDetector_Reset((XnVObjectID)steadyDetector);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1GetDetectionDuration
(JNIEnv* env, jclass cls, jlong steadyDetector, jobject pDuration)
{
	XnUInt32 nDuration = 0;
	XnStatus rc = xnvSteadyDetector_GetDetectionDuration((XnVObjectID)steadyDetector, &nDuration);
	SetOutArgIntValue(env, pDuration, nDuration);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1GetMaximumStdDevForSteady
(JNIEnv* env, jclass cls, jlong steadyDetector, jobject pDeviation)
{
	XnFloat fDeviation = 0;
	XnStatus rc = xnvSteadyDetector_GetMaximumStdDevForSteady((XnVObjectID)steadyDetector, &fDeviation);
	SetOutArgFloatValue(env, pDeviation, fDeviation);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1GetMinimumStdDevForNotSteady
(JNIEnv* env, jclass cls, jlong steadyDetector, jobject pDeviation)
{
	XnFloat fDeviation = 0;
	XnStatus rc = xnvSteadyDetector_GetMinimumStdDevForNotSteady((XnVObjectID)steadyDetector, &fDeviation);
	SetOutArgFloatValue(env, pDeviation, fDeviation);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1SetMaximumStdDevForSteady
(JNIEnv* env, jclass cls, jlong steadyDetector, jfloat deviation)
{
	return xnvSteadyDetector_SetMaximumStdDevForSteady((XnVObjectID)steadyDetector, deviation);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1SetMinimumStdDevForNotSteady
(JNIEnv* env, jclass cls, jlong steadyDetector, jfloat deviation)
{
	return xnvSteadyDetector_SetMinimumStdDevForNotSteady((XnVObjectID)steadyDetector, deviation);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1SetDetectionDuration
(JNIEnv* env, jclass cls, jlong steadyDetector, jint duration)
{
	return xnvSteadyDetector_SetDetectionDuration((XnVObjectID)steadyDetector, duration);
}

/// SteadyDetector::Steady
void XN_CALLBACK_TYPE SteadyDetectorSteadyHandler(XnUInt32 id, float deviation, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), id, deviation);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1RegisterSteady
(JNIEnv *env, jclass cls, jlong steadyDetector, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(IF)V");
	XnStatus rc = xnvSteadyDetector_RegisterSteady((XnVObjectID)steadyDetector, SteadyDetectorSteadyHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1UnregisterSteady
(JNIEnv *env, jclass cls, jlong steadyDetector, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSteadyDetector_UnregisterSteady((XnVObjectID)steadyDetector, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SteadyDetector::NotSteady
void XN_CALLBACK_TYPE SteadyDetectorNotSteadyHandler(XnUInt32 id, float deviation, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), id, deviation);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1RegisterNotSteady
(JNIEnv *env, jclass cls, jlong steadyDetector, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(IF)V");
	XnStatus rc = xnvSteadyDetector_RegisterNotSteady((XnVObjectID)steadyDetector, SteadyDetectorNotSteadyHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSteadyDetector_1UnregisterNotSteady
(JNIEnv *env, jclass cls, jlong steadyDetector, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSteadyDetector_UnregisterNotSteady((XnVObjectID)steadyDetector, pCookie->hCallback);
	delete pCookie;

	return rc;
}
//---------------------------------------------------------------------------
// SwipeDetector
//---------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1Create
  (JNIEnv* env, jclass cls, jboolean useSteady, jstring name)
{
	JavaString jName(env, name);
	return (jlong)xnvSwipeDetector_Create(useSteady, jName);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1Destroy
  (JNIEnv* env, jclass cls, jlong swipeDetector)
{
	return xnvSwipeDetector_Destroy((XnVObjectID)swipeDetector);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1Reset
(JNIEnv *env, jclass cls, jlong swipeDetector)
{
	return xnvSwipeDetector_Reset((XnVObjectID)swipeDetector);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1SetMotionSpeedThreshold
(JNIEnv* env, jclass cls, jlong swipeDetector, jfloat velocity)
{
	return xnvSwipeDetector_SetMotionSpeedThreshold((XnVObjectID)swipeDetector, velocity);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1SetMotionTime
(JNIEnv* env, jclass cls, jlong swipeDetector, jint duration)
{
	return xnvSwipeDetector_SetMotionTime((XnVObjectID)swipeDetector, duration);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1SetXAngleThreshold
(JNIEnv* env, jclass cls, jlong swipeDetector, jfloat angle)
{
	return xnvSwipeDetector_SetXAngleThreshold((XnVObjectID)swipeDetector, angle);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1SetYAngleThreshold
(JNIEnv* env, jclass cls, jlong swipeDetector, jfloat angle)
{
	return xnvSwipeDetector_SetYAngleThreshold((XnVObjectID)swipeDetector, angle);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1SetSteadyMaxStdDev
(JNIEnv* env, jclass cls, jlong swipeDetector, jfloat deviation)
{
	return xnvSwipeDetector_SetSteadyMaxStdDev((XnVObjectID)swipeDetector, deviation);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1SetSteadyDuration
(JNIEnv* env, jclass cls, jlong swipeDetector, jint duration)
{
	return xnvSwipeDetector_SetSteadyDuration((XnVObjectID)swipeDetector, duration);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1SetUseSteady
(JNIEnv* env, jclass cls, jlong swipeDetector, jboolean use)
{
	return xnvSwipeDetector_SetUseSteady((XnVObjectID)swipeDetector, use);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1GetMotionSpeedThreshold
(JNIEnv* env, jclass cls, jlong swipeDetector, jobject pVelocity)
{
	XnFloat fVelocity = 0;
	XnStatus rc = xnvSwipeDetector_GetMotionSpeedThreshold((XnVObjectID)swipeDetector, &fVelocity);
	SetOutArgFloatValue(env, pVelocity, fVelocity);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1GetMotionTime
(JNIEnv* env, jclass cls, jlong swipeDetector, jobject pDuration)
{
	XnUInt32 nDuration = 0;
	XnStatus rc = xnvSwipeDetector_GetMotionTime((XnVObjectID)swipeDetector, &nDuration);
	SetOutArgIntValue(env, pDuration, nDuration);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1GetXAngleThreshold
(JNIEnv* env, jclass cls, jlong swipeDetector, jobject pAngle)
{
	XnFloat fAngle = 0;
	XnStatus rc = xnvSwipeDetector_GetXAngleThreshold((XnVObjectID)swipeDetector, &fAngle);
	SetOutArgFloatValue(env, pAngle, fAngle);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1GetYAngleThreshold
(JNIEnv* env, jclass cls, jlong swipeDetector, jobject pAngle)
{
	XnFloat fAngle = 0;
	XnStatus rc = xnvSwipeDetector_GetYAngleThreshold((XnVObjectID)swipeDetector, &fAngle);
	SetOutArgFloatValue(env, pAngle, fAngle);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1GetSteadyMaxStdDev
(JNIEnv* env, jclass cls, jlong swipeDetector, jobject pDeviation)
{
	XnFloat fDeviation = 0;
	XnStatus rc = xnvSwipeDetector_GetSteadyMaxStdDev((XnVObjectID)swipeDetector, &fDeviation);
	SetOutArgFloatValue(env, pDeviation, fDeviation);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1GetSteadyDuration
(JNIEnv* env, jclass cls, jlong swipeDetector, jobject pDuration)
{
	XnUInt32 nDuration = 0;
	XnStatus rc = xnvSwipeDetector_GetSteadyDuration((XnVObjectID)swipeDetector, &nDuration);
	SetOutArgIntValue(env, pDuration, nDuration);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1GetUseSteady
(JNIEnv* env, jclass cls, jlong swipeDetector, jobject pUse)
{
	XnBool bUse = FALSE;
	XnStatus rc = xnvSwipeDetector_GetUseSteady((XnVObjectID)swipeDetector, &bUse);
	SetOutArgBooleanValue(env, pUse, bUse);
	return rc;
}

/// SwipeDetector::Swipe
void XN_CALLBACK_TYPE SwipeDetectorGeneralSwipeHandler(XnVDirection eDir, XnFloat velocity, XnFloat angle, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), eDir, velocity, angle);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1RegisterGeneralSwipe
(JNIEnv *env, jclass cls, jlong swipeDetector, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(IFF)V");
	XnStatus rc = xnvSwipeDetector_RegisterSwipe((XnVObjectID)swipeDetector, SwipeDetectorGeneralSwipeHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1UnregisterGeneralSwipe
(JNIEnv *env, jclass cls, jlong swipeDetector, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSwipeDetector_UnregisterSwipe((XnVObjectID)swipeDetector, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SwipeDetector::SwipeUp
void XN_CALLBACK_TYPE SwipeDetectorDirectionalSwipeHandler(XnFloat velocity, XnFloat angle, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), velocity, angle);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1RegisterSwipeUp
(JNIEnv *env, jclass cls, jlong swipeDetector, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(FF)V");
	XnStatus rc = xnvSwipeDetector_RegisterSwipeUp((XnVObjectID)swipeDetector, SwipeDetectorDirectionalSwipeHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1UnregisterSwipeUp
(JNIEnv *env, jclass cls, jlong swipeDetector, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSwipeDetector_UnregisterSwipeUp((XnVObjectID)swipeDetector, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SwipeDetector::SwipeDown
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1RegisterSwipeDown
(JNIEnv *env, jclass cls, jlong swipeDetector, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(FF)V");
	XnStatus rc = xnvSwipeDetector_RegisterSwipeDown((XnVObjectID)swipeDetector, SwipeDetectorDirectionalSwipeHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1UnregisterSwipeDown
(JNIEnv *env, jclass cls, jlong swipeDetector, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSwipeDetector_UnregisterSwipeDown((XnVObjectID)swipeDetector, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SwipeDetector::SwipeLeft
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1RegisterSwipeLeft
(JNIEnv *env, jclass cls, jlong swipeDetector, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(FF)V");
	XnStatus rc = xnvSwipeDetector_RegisterSwipeLeft((XnVObjectID)swipeDetector, SwipeDetectorDirectionalSwipeHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1UnregisterSwipeLeft
(JNIEnv *env, jclass cls, jlong swipeDetector, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSwipeDetector_UnregisterSwipeLeft((XnVObjectID)swipeDetector, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// SwipeDetector::SwipeRight
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1RegisterSwipeRight
(JNIEnv *env, jclass cls, jlong swipeDetector, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(FF)V");
	XnStatus rc = xnvSwipeDetector_RegisterSwipeRight((XnVObjectID)swipeDetector, SwipeDetectorDirectionalSwipeHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvSwipeDetector_1UnregisterSwipeRight
(JNIEnv *env, jclass cls, jlong swipeDetector, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvSwipeDetector_UnregisterSwipeRight((XnVObjectID)swipeDetector, pCookie->hCallback);
	delete pCookie;

	return rc;
}

//---------------------------------------------------------------------------
// PointFilter
//---------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointFilter_1Create
  (JNIEnv* env, jclass cls, jstring name)
{
	JavaString jName(env, name);
	return (jlong)xnvPointFilter_Create(jName);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointFilter_1Destroy
  (JNIEnv* env, jclass cls, jlong pointFilter)
{
	return xnvPointFilter_Destroy((XnVObjectID)pointFilter);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointFilter_1OverridePrimary
(JNIEnv *env, jclass cls, jlong pointFilter, jint newPrimary)
{
	return xnvPointFilter_OverridePrimary((XnVObjectID)pointFilter, newPrimary);
}

//---------------------------------------------------------------------------
// PointDenoiser
//---------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointDenoiser_1Create
  (JNIEnv* env, jclass cls, jfloat threshold, jstring name)
{
	JavaString jName(env, name);
	return (jlong)xnvPointDenoiser_Create(threshold, jName);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointDenoiser_1Destroy
  (JNIEnv* env, jclass cls, jlong pointDenoiser)
{
	return xnvPointDenoiser_Destroy((XnVObjectID)pointDenoiser);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointDenoiser_1SetDistanceThreshold
(JNIEnv* env, jclass cls, jlong pointDenoiser, jfloat threshold)
{
	return xnvPointDenoiser_SetDistanceThreshold((XnVObjectID)pointDenoiser, threshold);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointDenoiser_1GetDistanceThreshold
(JNIEnv* env, jclass cls, jlong pointDenoiser, jobject pTreshold)
{
	XnFloat fThreshold = 0;
	XnStatus rc = xnvPointDenoiser_GetDistanceThreshold((XnVObjectID)pointDenoiser, &fThreshold);
	SetOutArgFloatValue(env, pTreshold, fThreshold);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointDenoiser_1SetCloseRatio
(JNIEnv* env, jclass cls, jlong pointDenoiser, jfloat ratio)
{
	return xnvPointDenoiser_SetCloseRatio((XnVObjectID)pointDenoiser, ratio);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointDenoiser_1GetCloseRatio
(JNIEnv* env, jclass cls, jlong pointDenoiser, jobject pRatio)
{
	XnFloat fRatio = 0;
	XnStatus rc = xnvPointDenoiser_GetCloseRatio((XnVObjectID)pointDenoiser, &fRatio);
	SetOutArgFloatValue(env, pRatio, fRatio);
	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointDenoiser_1SetFarRatio
(JNIEnv* env, jclass cls, jlong pointDenoiser, jfloat ratio)
{
	return xnvPointDenoiser_SetFarRatio((XnVObjectID)pointDenoiser, ratio);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointDenoiser_1GetFarRatio
(JNIEnv* env, jclass cls, jlong pointDenoiser, jobject pRatio)
{
	XnFloat fRatio = 0;
	XnStatus rc = xnvPointDenoiser_GetFarRatio((XnVObjectID)pointDenoiser, &fRatio);
	SetOutArgFloatValue(env, pRatio, fRatio);
	return rc;
}

//---------------------------------------------------------------------------
// PointArea
//---------------------------------------------------------------------------

JNIEXPORT jlong JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointArea_1Create
(JNIEnv* env, jclass cls, jfloat minx, jfloat miny, jfloat minz, jfloat maxx, jfloat maxy, jfloat maxz, jboolean relative, jstring name)
{
	JavaString jName(env, name);
 	XnPoint3D mins = xnCreatePoint3D(minx, miny, minz);
 	XnPoint3D maxs = xnCreatePoint3D(maxx, maxy, maxz);
 	return (jlong)xnvPointArea_CreateByPoints(&mins, &maxs, relative, jName);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointArea_1Destroy
(JNIEnv* env, jclass cls, jlong pointArea)
{
	return xnvPointArea_Destroy((XnVObjectID)pointArea);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointArea_1ChangeArea
(JNIEnv* env, jclass cls, jlong pointArea, jfloat minx, jfloat miny, jfloat minz, jfloat maxx, jfloat maxy, jfloat maxz, jboolean relative)
{
	XnPoint3D mins = xnCreatePoint3D(minx, miny, minz);
	XnPoint3D maxs = xnCreatePoint3D(maxx, maxy, maxz);
	return (jlong)xnvPointArea_ChangeAreaByPoints((XnVObjectID)pointArea, &mins, &maxs, relative);
}

JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointArea_1RemoveSilent
(JNIEnv* env, jclass cls, jlong pointArea, jlong delay, jlong sessionManager)
{
	return xnvPointArea_RemoveSilent((XnVObjectID)pointArea, delay, (XnVObjectID)sessionManager);
}
/// PointArea::PointSilenced
void XN_CALLBACK_TYPE PointAreaPointSilencedHandler(XnUInt32 nId, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), nId);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointArea_1RegisterPointSilenced
(JNIEnv *env, jclass cls, jlong pointArea, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(I)V");
	XnStatus rc = xnvPointArea_RegisterToPointSilenced((XnVObjectID)pointArea, PointAreaPointSilencedHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointArea_1UnregisterPointSilenced
(JNIEnv *env, jclass cls, jlong pointArea, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPointArea_UnregisterFromPointSilenced((XnVObjectID)pointArea, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// PointArea::PointRevived
void XN_CALLBACK_TYPE PointAreaPointRevivedHandler(XnUInt32 nId, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), nId);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointArea_1RegisterPointRevived
(JNIEnv *env, jclass cls, jlong pointArea, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(I)V");
	XnStatus rc = xnvPointArea_RegisterToPointRevived((XnVObjectID)pointArea, PointAreaPointRevivedHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointArea_1UnregisterPointRevived
(JNIEnv *env, jclass cls, jlong pointArea, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPointArea_UnregisterFromPointRevived((XnVObjectID)pointArea, pCookie->hCallback);
	delete pCookie;

	return rc;
}
/// PointArea::SilentPointRemoved
void XN_CALLBACK_TYPE PointAreaSilentPointRemovedHandler(XnUInt32 nId, void* pCookie)
{
	CallbackCookie* pCallback = (CallbackCookie*)pCookie;

	JNIEnvSupplier().GetEnv()->CallVoidMethod(pCallback->GetObject(), pCallback->GetMethodID(), nId);
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointArea_1RegisterSilentPointRemoved
(JNIEnv *env, jclass cls, jlong pointArea, jobject obj, jstring cb, jobject hCallback)
{
	CallbackCookie* pCookie = new CallbackCookie(env, obj, cb, "(I)V");
	XnStatus rc = xnvPointArea_RegisterToSilentPointRemoved((XnVObjectID)pointArea, PointAreaSilentPointRemovedHandler, pCookie, &pCookie->hCallback);
	SetOutArgPointerValue(env, hCallback, pCookie);

	return rc;
}
JNIEXPORT jint JNICALL Java_com_primesense_NiteControls_NativeMethods_xnvPointArea_1UnregisterSilentPointRemoved
(JNIEnv *env, jclass cls, jlong pointArea, jlong hCallback)
{
	CallbackCookie* pCookie = (CallbackCookie*)hCallback;
	XnStatus rc = xnvPointArea_UnregisterFromSilentPointRemoved((XnVObjectID)pointArea, pCookie->hCallback);
	delete pCookie;

	return rc;
}
