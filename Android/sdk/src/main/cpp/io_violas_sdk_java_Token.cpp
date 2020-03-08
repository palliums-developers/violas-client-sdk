//
// Created by hunter on 20-3-2.
//
#include <jni.h>
#include <vector>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include "jni_violas_sdk.h"

extern "C" {

void init_all_script(JNIEnv *env, jobject obj, jobject assetManager) {
	AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
	AAssetDir *top_dir = AAssetManager_openDir(mgr, ""); //open the top-level directory

	if (!top_dir)
		return;

	std::vector<uint8_t> buffer;

	for (const char *file_name = AAssetDir_getNextFileName(top_dir);
	     file_name != nullptr;
	     file_name = AAssetDir_getNextFileName(top_dir)) {

		AAsset *file = AAssetManager_open(mgr, file_name, AASSET_MODE_BUFFER);
		size_t length = AAsset_getLength64(file);

		buffer.resize(length);
		AAsset_read(file, buffer.data(), length);

		AAsset_close(file);
	}

	AAssetDir_close(top_dir);

}
/*
 * Class:     io_violas_sdk_java_Token
 * Method:    nativeCreateToken1
 * Signature: (J[BLjava/lang/String;Landroid/content/res/AssetManager;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_io_violas_sdk_java_Token_nativeCreateToken1
		(JNIEnv *env, jobject obj, jlong, jbyteArray, jstring, jobject assetMgr, jstring) {

	init_all_script(env, obj, assetMgr);

	return 0;
}
/*
 * Class:     io_violas_sdk_java_Token
 * Method:    nativeCreateToken
 * Signature: (J[BLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_io_violas_sdk_java_Token_nativeCreateToken
		(JNIEnv *env, jobject obj, jlong native_client, jbyteArray publisher_address,
		 jstring token_name, jstring script_files_path, jstring temp_path) {
	return Jni_Token_Wrapper::jni_create_totken(env, obj, native_client, publisher_address,
	                                            token_name, script_files_path, temp_path);
}

/*
 * Class:     io_violas_sdk_java_Token
 * Method:    nativeName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_io_violas_sdk_java_Token_nativeName
		(JNIEnv *env, jobject obj, jlong native_token) {
	return Jni_Token_Wrapper::jni_name(env, obj, native_token);
}

/*
 * Class:     io_violas_sdk_java_Token
 * Method:    nativeAddress
 * Signature: (J)[B
 */
JNIEXPORT jbyteArray JNICALL Java_io_violas_sdk_java_Token_nativeAddress
		(JNIEnv *env, jobject obj, jlong native_token) {
	return Jni_Token_Wrapper::jni_address(env, obj, native_token);
}

/*
 * Class:     io_violas_sdk_java_Token
 * Method:    nativeDeploy
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_io_violas_sdk_java_Token_nativeDeploy
		(JNIEnv *env, jobject obj, jlong native_token, jlong account_index) {
	Jni_Token_Wrapper::jni_deploy(env, obj, native_token, account_index);
}

/*
 * Class:     io_violas_sdk_java_Token
 * Method:    nativePublish
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_io_violas_sdk_java_Token_nativePublish
		(JNIEnv *env, jobject obj, jlong native_token, jlong account_index) {
	Jni_Token_Wrapper::jni_publish(env, obj, native_token, account_index);
}

/*
 * Class:     io_violas_sdk_java_Token
 * Method:    nativeMint
 * Signature: (JJ[BJ)V
 */
JNIEXPORT void JNICALL Java_io_violas_sdk_java_Token_nativeMint
		(JNIEnv *env, jobject obj, jlong native_token, jlong account_index,
		 jbyteArray receiver_address, jlong amount_micro_coins) {
	Jni_Token_Wrapper::jni_mint(env, obj, native_token, account_index, receiver_address,
	                            amount_micro_coins);
}

/*
 * Class:     io_violas_sdk_java_Token
 * Method:    nativeTransfer
 * Signature: (JJ[BJ)V
 */
JNIEXPORT void JNICALL Java_io_violas_sdk_java_Token_nativeTransfer
		(JNIEnv *env, jobject obj, jlong native_token, jlong account_index,
		 jbyteArray receiver_address, jlong amount_micro_coins) {
	Jni_Token_Wrapper::jni_mint(env, obj, native_token, account_index, receiver_address,
	                            amount_micro_coins);
}

/*
 * Class:     io_violas_sdk_java_Token
 * Method:    nativeGetBalance
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_io_violas_sdk_java_Token_nativeGetBalance__JJ
		(JNIEnv *env, jobject obj, jlong native_token, jlong account_index) {
	return Jni_Token_Wrapper::jni_get_balance(env, obj, native_token, account_index);
}

/*
 * Class:     io_violas_sdk_java_Token
 * Method:    nativeGetBalance
 * Signature: (J[B)J
 */
JNIEXPORT jlong JNICALL Java_io_violas_sdk_java_Token_nativeGetBalance__J_3B
		(JNIEnv *env, jobject obj, jlong native_token, jbyteArray address) {
	return Jni_Token_Wrapper::jni_get_balance(env, obj, native_token, address);
}


} // end of extern "C"