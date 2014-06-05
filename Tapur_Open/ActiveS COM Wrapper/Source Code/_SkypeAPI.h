

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jun 05 09:30:27 2014
 */
/* Compiler settings for _SkypeAPI.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef ___SkypeAPI_h__
#define ___SkypeAPI_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IConversion_FWD_DEFINED__
#define __IConversion_FWD_DEFINED__
typedef interface IConversion IConversion;
#endif 	/* __IConversion_FWD_DEFINED__ */


#ifndef __ICallCollection_FWD_DEFINED__
#define __ICallCollection_FWD_DEFINED__
typedef interface ICallCollection ICallCollection;
#endif 	/* __ICallCollection_FWD_DEFINED__ */


#ifndef __ICall_FWD_DEFINED__
#define __ICall_FWD_DEFINED__
typedef interface ICall ICall;
#endif 	/* __ICall_FWD_DEFINED__ */


#ifndef __IUserCollection_FWD_DEFINED__
#define __IUserCollection_FWD_DEFINED__
typedef interface IUserCollection IUserCollection;
#endif 	/* __IUserCollection_FWD_DEFINED__ */


#ifndef __IUser_FWD_DEFINED__
#define __IUser_FWD_DEFINED__
typedef interface IUser IUser;
#endif 	/* __IUser_FWD_DEFINED__ */


#ifndef __IApp2App_FWD_DEFINED__
#define __IApp2App_FWD_DEFINED__
typedef interface IApp2App IApp2App;
#endif 	/* __IApp2App_FWD_DEFINED__ */


#ifndef __IApp2AppStream_FWD_DEFINED__
#define __IApp2AppStream_FWD_DEFINED__
typedef interface IApp2AppStream IApp2AppStream;
#endif 	/* __IApp2AppStream_FWD_DEFINED__ */


#ifndef __IMessageCollection_FWD_DEFINED__
#define __IMessageCollection_FWD_DEFINED__
typedef interface IMessageCollection IMessageCollection;
#endif 	/* __IMessageCollection_FWD_DEFINED__ */


#ifndef __IMessage_FWD_DEFINED__
#define __IMessage_FWD_DEFINED__
typedef interface IMessage IMessage;
#endif 	/* __IMessage_FWD_DEFINED__ */


#ifndef __IChatMessageCollection_FWD_DEFINED__
#define __IChatMessageCollection_FWD_DEFINED__
typedef interface IChatMessageCollection IChatMessageCollection;
#endif 	/* __IChatMessageCollection_FWD_DEFINED__ */


#ifndef __IChatMessage_FWD_DEFINED__
#define __IChatMessage_FWD_DEFINED__
typedef interface IChatMessage IChatMessage;
#endif 	/* __IChatMessage_FWD_DEFINED__ */


#ifndef __IChatCollection_FWD_DEFINED__
#define __IChatCollection_FWD_DEFINED__
typedef interface IChatCollection IChatCollection;
#endif 	/* __IChatCollection_FWD_DEFINED__ */


#ifndef __IChat_FWD_DEFINED__
#define __IChat_FWD_DEFINED__
typedef interface IChat IChat;
#endif 	/* __IChat_FWD_DEFINED__ */


#ifndef __IVoiceMailCollection_FWD_DEFINED__
#define __IVoiceMailCollection_FWD_DEFINED__
typedef interface IVoiceMailCollection IVoiceMailCollection;
#endif 	/* __IVoiceMailCollection_FWD_DEFINED__ */


#ifndef __IVoiceMail_FWD_DEFINED__
#define __IVoiceMail_FWD_DEFINED__
typedef interface IVoiceMail IVoiceMail;
#endif 	/* __IVoiceMail_FWD_DEFINED__ */


#ifndef __IUI_FWD_DEFINED__
#define __IUI_FWD_DEFINED__
typedef interface IUI IUI;
#endif 	/* __IUI_FWD_DEFINED__ */


#ifndef __IProfile_FWD_DEFINED__
#define __IProfile_FWD_DEFINED__
typedef interface IProfile IProfile;
#endif 	/* __IProfile_FWD_DEFINED__ */


#ifndef __IAccess_FWD_DEFINED__
#define __IAccess_FWD_DEFINED__
typedef interface IAccess IAccess;
#endif 	/* __IAccess_FWD_DEFINED__ */


#ifndef __Conversion_FWD_DEFINED__
#define __Conversion_FWD_DEFINED__

#ifdef __cplusplus
typedef class Conversion Conversion;
#else
typedef struct Conversion Conversion;
#endif /* __cplusplus */

#endif 	/* __Conversion_FWD_DEFINED__ */


#ifndef __CallCollection_FWD_DEFINED__
#define __CallCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class CallCollection CallCollection;
#else
typedef struct CallCollection CallCollection;
#endif /* __cplusplus */

#endif 	/* __CallCollection_FWD_DEFINED__ */


#ifndef __Call_FWD_DEFINED__
#define __Call_FWD_DEFINED__

#ifdef __cplusplus
typedef class Call Call;
#else
typedef struct Call Call;
#endif /* __cplusplus */

#endif 	/* __Call_FWD_DEFINED__ */


#ifndef __UserCollection_FWD_DEFINED__
#define __UserCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class UserCollection UserCollection;
#else
typedef struct UserCollection UserCollection;
#endif /* __cplusplus */

#endif 	/* __UserCollection_FWD_DEFINED__ */


#ifndef __User_FWD_DEFINED__
#define __User_FWD_DEFINED__

#ifdef __cplusplus
typedef class User User;
#else
typedef struct User User;
#endif /* __cplusplus */

#endif 	/* __User_FWD_DEFINED__ */


#ifndef __App2App_FWD_DEFINED__
#define __App2App_FWD_DEFINED__

#ifdef __cplusplus
typedef class App2App App2App;
#else
typedef struct App2App App2App;
#endif /* __cplusplus */

#endif 	/* __App2App_FWD_DEFINED__ */


#ifndef __App2AppStream_FWD_DEFINED__
#define __App2AppStream_FWD_DEFINED__

#ifdef __cplusplus
typedef class App2AppStream App2AppStream;
#else
typedef struct App2AppStream App2AppStream;
#endif /* __cplusplus */

#endif 	/* __App2AppStream_FWD_DEFINED__ */


#ifndef __MessageCollection_FWD_DEFINED__
#define __MessageCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class MessageCollection MessageCollection;
#else
typedef struct MessageCollection MessageCollection;
#endif /* __cplusplus */

#endif 	/* __MessageCollection_FWD_DEFINED__ */


#ifndef __Message_FWD_DEFINED__
#define __Message_FWD_DEFINED__

#ifdef __cplusplus
typedef class Message Message;
#else
typedef struct Message Message;
#endif /* __cplusplus */

#endif 	/* __Message_FWD_DEFINED__ */


#ifndef __ChatMessageCollection_FWD_DEFINED__
#define __ChatMessageCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class ChatMessageCollection ChatMessageCollection;
#else
typedef struct ChatMessageCollection ChatMessageCollection;
#endif /* __cplusplus */

#endif 	/* __ChatMessageCollection_FWD_DEFINED__ */


#ifndef __ChatMessage_FWD_DEFINED__
#define __ChatMessage_FWD_DEFINED__

#ifdef __cplusplus
typedef class ChatMessage ChatMessage;
#else
typedef struct ChatMessage ChatMessage;
#endif /* __cplusplus */

#endif 	/* __ChatMessage_FWD_DEFINED__ */


#ifndef __ChatCollection_FWD_DEFINED__
#define __ChatCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class ChatCollection ChatCollection;
#else
typedef struct ChatCollection ChatCollection;
#endif /* __cplusplus */

#endif 	/* __ChatCollection_FWD_DEFINED__ */


#ifndef __Chat_FWD_DEFINED__
#define __Chat_FWD_DEFINED__

#ifdef __cplusplus
typedef class Chat Chat;
#else
typedef struct Chat Chat;
#endif /* __cplusplus */

#endif 	/* __Chat_FWD_DEFINED__ */


#ifndef __VoiceMailCollection_FWD_DEFINED__
#define __VoiceMailCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoiceMailCollection VoiceMailCollection;
#else
typedef struct VoiceMailCollection VoiceMailCollection;
#endif /* __cplusplus */

#endif 	/* __VoiceMailCollection_FWD_DEFINED__ */


#ifndef __VoiceMail_FWD_DEFINED__
#define __VoiceMail_FWD_DEFINED__

#ifdef __cplusplus
typedef class VoiceMail VoiceMail;
#else
typedef struct VoiceMail VoiceMail;
#endif /* __cplusplus */

#endif 	/* __VoiceMail_FWD_DEFINED__ */


#ifndef __UI_FWD_DEFINED__
#define __UI_FWD_DEFINED__

#ifdef __cplusplus
typedef class UI UI;
#else
typedef struct UI UI;
#endif /* __cplusplus */

#endif 	/* __UI_FWD_DEFINED__ */


#ifndef __Profile_FWD_DEFINED__
#define __Profile_FWD_DEFINED__

#ifdef __cplusplus
typedef class Profile Profile;
#else
typedef struct Profile Profile;
#endif /* __cplusplus */

#endif 	/* __Profile_FWD_DEFINED__ */


#ifndef ___IAccessEvents_FWD_DEFINED__
#define ___IAccessEvents_FWD_DEFINED__
typedef interface _IAccessEvents _IAccessEvents;
#endif 	/* ___IAccessEvents_FWD_DEFINED__ */


#ifndef __Access_FWD_DEFINED__
#define __Access_FWD_DEFINED__

#ifdef __cplusplus
typedef class Access Access;
#else
typedef struct Access Access;
#endif /* __cplusplus */

#endif 	/* __Access_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"
#include "ForwardDefs.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf__SkypeAPI_0000_0000 */
/* [local] */ 

/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("65BB3F87-9AC8-456e-BB0B-3C8034FD1371") SkypeAPIAttachmentStatus
    {	apiAttachUnknown	= -1,
	apiAttachSuccess	= 0,
	apiAttachPendingAuth	= 1,
	apiAttachRefused	= 2,
	apiAttachNotAvailable	= 3,
	apiAttachAPIAvail	= 32769
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("8FCBDB89-66FF-4638-ABB2-CD9B41CE70D7") SkypeConnectionStatus
    {	conUnknown	= -1,
	conOffline	= 0,
	conConnecting	= 1,
	conPausing	= 2,
	conOnline	= 3,
	conLoggedOut	= 4
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("A92E0B9F-DAF6-4092-A772-E920FEDDCCE9") SkypeCallProgress
    {	prgUnknown	= -1,
	prgUnplaced	= 0,
	prgRouting	= 1,
	prgEarlyMedia	= 2,
	prgFailed	= 3,
	prgRinging	= 4,
	prgInProgress	= 5,
	prgOnHold	= 6,
	prgFinished	= 7,
	prgMissed	= 8,
	prgRefused	= 9,
	prgBusy	= 10,
	prgCancelled	= 11,
	prgRemoteHold	= 12,
	prgLocalHold	= 13,
	prgBufferingGreeting	= 14,
	prgPlayingGreeting	= 15,
	prgRecordingVoiceMail	= 16,
	prgUploadingVoiceMail	= 17,
	prgSentVoiceMail	= 18,
	prgCancelledVoiceMail	= 19,
	prgFailedVoiceMail	= 20
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("3859796A-5B13-4400-80B2-254516AEC067") SkypeCallType
    {	ctypUnknown	= -1,
	ctypIncomingPSTN	= 0,
	ctypOutgoingPSTN	= 1,
	ctypIncomingP2P	= 2,
	ctypOutgoingP2P	= 3
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("33C4110A-02D8-4092-AC69-B7B258786762") SkypeMessageType
    {	mtypUnknown	= -1,
	mtypAuthRequest	= 0,
	mtypText	= 1,
	mtypContacts	= 2
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("D6E5321C-982A-4470-9F50-D5E97280D2E0") SkypeMessageStatus
    {	msgUnknown	= -1,
	msgSending	= 0,
	msgSent	= 1,
	msgFailed	= 2,
	msgReceived	= 3,
	msgRead	= 4,
	msgIgnored	= 5,
	msgQueued	= 6
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("127D4942-E520-4cfc-B5FD-B2FDFBA98782") SkypeUserSex
    {	usexUnknown	= -1,
	usexNotSpecified	= 0,
	usexMale	= 1,
	usexFemale	= 2
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("1D72D739-69FB-48b0-9D2D-E9FA0E1108AE") SkypeBuddyStatus
    {	budUnknown	= -1,
	budNever	= 0,
	budDeleted	= 1,
	budPending	= 2,
	budFriend	= 3
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("BD1F7281-2E21-4d6c-993E-2C21B1114726") SkypeOnlineStatus
    {	olsUnknown	= -1,
	olsOffline	= 0,
	olsOnline	= 1,
	olsAway	= 2,
	olsNotAvail	= 3,
	olsDoNotDisturb	= 4,
	olsInvisible	= 5,
	olsSkypeMe	= 6,
	olsLoggedOut	= 7
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("8DD2EDDA-C8FE-4c37-8B49-FE04825C8206") SkypeChatStatus
    {	chatUnknown	= -1,
	chatLegacyDialog	= 0,
	chatDialog	= 1,
	chatMultiNeedAccept	= 2,
	chatMultiSubscribed	= 3
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("5F10C769-6EF1-4495-9E12-B37ED9C311D4") SkypeChatMessageStatus
    {	chatMsgUnknown	= -1,
	chatMsgSending	= 0,
	chatMsgSent	= 1,
	chatMsgReceived	= 2,
	chatMsgRead	= 3
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("2315D3E3-C08A-4a5d-8A87-5D9439DCBFFC") SkypeChatMessageLeftReason
    {	chatMsgLeftUnknown	= -1,
	chatMsgLeftUserNotFound	= 0,
	chatMsgLeftUserIncapable	= 1,
	chatMsgLeftAdderMustBeFriend	= 2,
	chatMsgLeftAdderMustBeAuthorized	= 3,
	chatMsgLeftDeclineAdd	= 4,
	chatMsgLeftUnsubscribe	= 5
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("380437AD-6069-4a9f-B53F-9FDB9D7FF6C6") SkypeCurrencyType
    {	currUnknown	= -1,
	currEuroCents	= 0
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("D700390A-F14B-4275-9FF7-5E19802BEF3D") SkypeChatMessageType
    {	chatMsgTypeUnknown	= -1,
	chatMsgTypeSetTopic	= 0,
	chatMsgTypeSaid	= 1,
	chatMsgTypeAddedMembers	= 2,
	chatMsgTypeSawMembers	= 3,
	chatMsgTypeCreatedChatWith	= 4,
	chatMsgTypeLeft	= 5
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("FCDD7B32-D901-4933-8C5D-1B380BDE3C96") SkypeVoiceMailType
    {	vmTypeUnknown	= -1,
	vmTypeIncoming	= 0,
	vmTypeDefaultGreeting	= 1,
	vmTypeCustomGreeting	= 2,
	vmTypeOutgoing	= 3
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("0B7F55D7-ED68-4e09-91A4-0F20DB0E186F") SkypeVoiceMailStatus
    {	vmStatusUnknown	= -1,
	vmStatusDownloading	= 0,
	vmStatusDownloaded	= 1,
	vmStatusUnplayed	= 2,
	vmStatusBuffering	= 3,
	vmStatusPlaying	= 4,
	vmStatusPlayed	= 5,
	vmStatusBlank	= 6,
	vmStatusRecording	= 7,
	vmStatusRecorded	= 8,
	vmStatusUploading	= 9,
	vmStatusUploaded	= 10,
	vmStatusDeleting	= 11,
	vmStatusFailed	= 12
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("57181448-1317-411c-B064-6F2E872F3BC0") SkypeVoiceMailFailure
    {	vmFailureUnknown	= -1,
	vmFailureMisc	= 0,
	vmFailureConnect	= 1,
	vmFailureNoPrivilege	= 2,
	vmFailureNoSuchVoiceMail	= 3,
	vmFailureReadError	= 4,
	vmFailureWriteError	= 5,
	vmFailureRecording	= 6,
	vmFailurePlayback	= 7
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("4D1434DB-6F9F-4533-ACAF-B712F9F54F81") SkypeClearCallHistory
    {	clrAllCalls	= 0,
	clrMissedCalls	= 1,
	clrIncomingCalls	= 2,
	clrOutgoingCalls	= 3
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("A3678CC3-AC6E-4ffb-A8B5-FA57B97D4BF4") SkypeNoAnswerAction
    {	noAnsUnknown	= -1,
	noAnsReject	= 0,
	noAnsForward	= 1,
	noAnsVoicemail	= 2
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("EEC83F5F-3413-4c9e-BACA-295A76125CD8") SkypeOptionsPage
    {	pgGeneral	= 0,
	pgPrivacy	= 1,
	pgNotifications	= 2,
	pgSoundAlerts	= 3,
	pgSoundDevices	= 4,
	pgHotKeys	= 5,
	pgConnection	= 6,
	pgVoiceMail	= 7,
	pgCallForward	= 8,
	pgVideo	= 9,
	pgAdvanced	= 10
    } ;
/* [helpcontext][uuid][v1_enum] */ 
enum  DECLSPEC_UUID("E8EA86FF-4282-4bf1-B371-EDDA7F97D5B7") SkypeRingtone
    {	rngRingtone	= 1101,
	rngRingback	= 1102,
	rngBusy	= 1103,
	rngDialing	= 1104,
	rngConnecting	= 1105,
	rngResume	= 1202,
	rngHangup	= 1203,
	rngIncomingMessage	= 1204,
	rngOnlineAlert	= 1205
    } ;


extern RPC_IF_HANDLE __MIDL_itf__SkypeAPI_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf__SkypeAPI_0000_0000_v0_0_s_ifspec;

#ifndef __IConversion_INTERFACE_DEFINED__
#define __IConversion_INTERFACE_DEFINED__

/* interface IConversion */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IConversion;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E943F65C-CAD7-4e98-B380-54BCA0AB8030")
    IConversion : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeCallProgressAsText( 
            /* [in] */ enum SkypeCallProgress Progress,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeCallTypeAsText( 
            /* [in] */ enum SkypeCallType CallType,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeMessageStatusAsText( 
            /* [in] */ enum SkypeMessageStatus Status,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeMessageTypeAsText( 
            /* [in] */ enum SkypeMessageType Type,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeUserSexAsText( 
            /* [in] */ enum SkypeUserSex Sex,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeBuddyStatusAsText( 
            /* [in] */ enum SkypeBuddyStatus Status,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeOnlineStatusAsText( 
            /* [in] */ enum SkypeOnlineStatus Status,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeConnectionStatusAsText( 
            /* [in] */ enum SkypeConnectionStatus Status,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeAPIAttachmentStatusAsText( 
            /* [in] */ enum SkypeAPIAttachmentStatus Status,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeChatStatusAsText( 
            /* [in] */ enum SkypeChatStatus ChatStatus,
            /* [out][retval] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeChatMessageStatusAsText( 
            /* [in] */ enum SkypeChatMessageStatus Status,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeChatMessageLeftReasonAsText( 
            /* [in] */ enum SkypeChatMessageLeftReason Reason,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeCurrencyTypeAsText( 
            /* [in] */ enum SkypeCurrencyType Currency,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeChatMessageTypeAsText( 
            /* [in] */ enum SkypeChatMessageType MessageType,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeVoiceMailTypeAsText( 
            /* [in] */ enum SkypeVoiceMailType VoiceMailType,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeVoiceMailStatusAsText( 
            /* [in] */ enum SkypeVoiceMailStatus VoiceMailStatus,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeVoiceMailFailureAsText( 
            /* [in] */ enum SkypeVoiceMailFailure FailureReason,
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SkypeNoAnswerActionAsText( 
            /* [in] */ enum SkypeNoAnswerAction Action,
            /* [retval][out] */ BSTR *Text) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConversionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IConversion * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IConversion * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IConversion * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IConversion * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IConversion * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IConversion * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IConversion * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeCallProgressAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeCallProgress Progress,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeCallTypeAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeCallType CallType,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeMessageStatusAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeMessageStatus Status,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeMessageTypeAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeMessageType Type,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeUserSexAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeUserSex Sex,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeBuddyStatusAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeBuddyStatus Status,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeOnlineStatusAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeOnlineStatus Status,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeConnectionStatusAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeConnectionStatus Status,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeAPIAttachmentStatusAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeAPIAttachmentStatus Status,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeChatStatusAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeChatStatus ChatStatus,
            /* [out][retval] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeChatMessageStatusAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeChatMessageStatus Status,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeChatMessageLeftReasonAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeChatMessageLeftReason Reason,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeCurrencyTypeAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeCurrencyType Currency,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeChatMessageTypeAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeChatMessageType MessageType,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeVoiceMailTypeAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeVoiceMailType VoiceMailType,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeVoiceMailStatusAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeVoiceMailStatus VoiceMailStatus,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeVoiceMailFailureAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeVoiceMailFailure FailureReason,
            /* [retval][out] */ BSTR *Text);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SkypeNoAnswerActionAsText )( 
            IConversion * This,
            /* [in] */ enum SkypeNoAnswerAction Action,
            /* [retval][out] */ BSTR *Text);
        
        END_INTERFACE
    } IConversionVtbl;

    interface IConversion
    {
        CONST_VTBL struct IConversionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConversion_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IConversion_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IConversion_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IConversion_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IConversion_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IConversion_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IConversion_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IConversion_SkypeCallProgressAsText(This,Progress,Text)	\
    ( (This)->lpVtbl -> SkypeCallProgressAsText(This,Progress,Text) ) 

#define IConversion_SkypeCallTypeAsText(This,CallType,Text)	\
    ( (This)->lpVtbl -> SkypeCallTypeAsText(This,CallType,Text) ) 

#define IConversion_SkypeMessageStatusAsText(This,Status,Text)	\
    ( (This)->lpVtbl -> SkypeMessageStatusAsText(This,Status,Text) ) 

#define IConversion_SkypeMessageTypeAsText(This,Type,Text)	\
    ( (This)->lpVtbl -> SkypeMessageTypeAsText(This,Type,Text) ) 

#define IConversion_SkypeUserSexAsText(This,Sex,Text)	\
    ( (This)->lpVtbl -> SkypeUserSexAsText(This,Sex,Text) ) 

#define IConversion_SkypeBuddyStatusAsText(This,Status,Text)	\
    ( (This)->lpVtbl -> SkypeBuddyStatusAsText(This,Status,Text) ) 

#define IConversion_SkypeOnlineStatusAsText(This,Status,Text)	\
    ( (This)->lpVtbl -> SkypeOnlineStatusAsText(This,Status,Text) ) 

#define IConversion_SkypeConnectionStatusAsText(This,Status,Text)	\
    ( (This)->lpVtbl -> SkypeConnectionStatusAsText(This,Status,Text) ) 

#define IConversion_SkypeAPIAttachmentStatusAsText(This,Status,Text)	\
    ( (This)->lpVtbl -> SkypeAPIAttachmentStatusAsText(This,Status,Text) ) 

#define IConversion_SkypeChatStatusAsText(This,ChatStatus,Text)	\
    ( (This)->lpVtbl -> SkypeChatStatusAsText(This,ChatStatus,Text) ) 

#define IConversion_SkypeChatMessageStatusAsText(This,Status,Text)	\
    ( (This)->lpVtbl -> SkypeChatMessageStatusAsText(This,Status,Text) ) 

#define IConversion_SkypeChatMessageLeftReasonAsText(This,Reason,Text)	\
    ( (This)->lpVtbl -> SkypeChatMessageLeftReasonAsText(This,Reason,Text) ) 

#define IConversion_SkypeCurrencyTypeAsText(This,Currency,Text)	\
    ( (This)->lpVtbl -> SkypeCurrencyTypeAsText(This,Currency,Text) ) 

#define IConversion_SkypeChatMessageTypeAsText(This,MessageType,Text)	\
    ( (This)->lpVtbl -> SkypeChatMessageTypeAsText(This,MessageType,Text) ) 

#define IConversion_SkypeVoiceMailTypeAsText(This,VoiceMailType,Text)	\
    ( (This)->lpVtbl -> SkypeVoiceMailTypeAsText(This,VoiceMailType,Text) ) 

#define IConversion_SkypeVoiceMailStatusAsText(This,VoiceMailStatus,Text)	\
    ( (This)->lpVtbl -> SkypeVoiceMailStatusAsText(This,VoiceMailStatus,Text) ) 

#define IConversion_SkypeVoiceMailFailureAsText(This,FailureReason,Text)	\
    ( (This)->lpVtbl -> SkypeVoiceMailFailureAsText(This,FailureReason,Text) ) 

#define IConversion_SkypeNoAnswerActionAsText(This,Action,Text)	\
    ( (This)->lpVtbl -> SkypeNoAnswerActionAsText(This,Action,Text) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IConversion_INTERFACE_DEFINED__ */


#ifndef __ICallCollection_INTERFACE_DEFINED__
#define __ICallCollection_INTERFACE_DEFINED__

/* interface ICallCollection */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICallCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D84C375F-43B9-446a-ADFB-F504FE049052")
    ICallCollection : public IDispatch
    {
    public:
        virtual /* [helpcontext][id][restricted][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppUnk) = 0;
        
        virtual /* [helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ ICall **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICallCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICallCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICallCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICallCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICallCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICallCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICallCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICallCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][id][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            ICallCollection * This,
            /* [retval][out] */ IUnknown **ppUnk);
        
        /* [helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            ICallCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            ICallCollection * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ ICall **pVal);
        
        END_INTERFACE
    } ICallCollectionVtbl;

    interface ICallCollection
    {
        CONST_VTBL struct ICallCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICallCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICallCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICallCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICallCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICallCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICallCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICallCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICallCollection_get__NewEnum(This,ppUnk)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppUnk) ) 

#define ICallCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define ICallCollection_get_Item(This,index,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,index,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICallCollection_INTERFACE_DEFINED__ */


#ifndef __ICall_INTERFACE_DEFINED__
#define __ICall_INTERFACE_DEFINED__

/* interface ICall */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICall;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("76B64C71-D601-4d72-A253-FCFE372ECC62")
    ICall : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Timestamp( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_PartnerHandle( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_PartnerDisplayName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ConferenceID( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ enum SkypeCallType *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ enum SkypeCallProgress *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Status( 
            /* [in] */ enum SkypeCallProgress newVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_FailureReason( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Subject( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_PSTN_Number( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Duration( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_PSTN_Status( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ParticipantCount( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetParticipantInformation( 
            /* [in] */ LONG Number,
            /* [out] */ BSTR *Handle,
            /* [out] */ enum SkypeCallType *Type,
            /* [out] */ enum SkypeCallProgress *Status,
            /* [out] */ BSTR *DisplayName) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE JoinConference( 
            /* [in] */ LONG CallID) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_VoiceMailDuration( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_VoiceMailAllowedDuration( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SendDTMF( 
            /* [in] */ BSTR Codes) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetInput( 
            /* [out] */ BSTR *Soundcard,
            /* [out] */ BSTR *Port,
            /* [out] */ BSTR *File) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetOutput( 
            /* [out] */ BSTR *Soundcard,
            /* [out] */ BSTR *Port,
            /* [out] */ BSTR *File) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetCaptureMic( 
            /* [out] */ BSTR *Port,
            /* [out] */ BSTR *File) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetInput( 
            /* [in] */ BSTR Soundcard,
            /* [in] */ BSTR Port,
            /* [in] */ BSTR File) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetOutput( 
            /* [in] */ BSTR Soundcard,
            /* [in] */ BSTR Port,
            /* [in] */ BSTR File) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetCaptureMic( 
            /* [in] */ BSTR Port,
            /* [in] */ BSTR File) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_VideoStatus( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE StartVideoSend( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE StopVideoSend( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE StartVideoRecieve( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE StopVideoRecieve( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICallVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICall * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICall * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICall * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICall * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICall * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICall * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICall * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ID )( 
            ICall * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Timestamp )( 
            ICall * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PartnerHandle )( 
            ICall * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PartnerDisplayName )( 
            ICall * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ConferenceID )( 
            ICall * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            ICall * This,
            /* [retval][out] */ enum SkypeCallType *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            ICall * This,
            /* [retval][out] */ enum SkypeCallProgress *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Status )( 
            ICall * This,
            /* [in] */ enum SkypeCallProgress newVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FailureReason )( 
            ICall * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Subject )( 
            ICall * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PSTN_Number )( 
            ICall * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Duration )( 
            ICall * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PSTN_Status )( 
            ICall * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ParticipantCount )( 
            ICall * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetParticipantInformation )( 
            ICall * This,
            /* [in] */ LONG Number,
            /* [out] */ BSTR *Handle,
            /* [out] */ enum SkypeCallType *Type,
            /* [out] */ enum SkypeCallProgress *Status,
            /* [out] */ BSTR *DisplayName);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *JoinConference )( 
            ICall * This,
            /* [in] */ LONG CallID);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceMailDuration )( 
            ICall * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceMailAllowedDuration )( 
            ICall * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SendDTMF )( 
            ICall * This,
            /* [in] */ BSTR Codes);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetInput )( 
            ICall * This,
            /* [out] */ BSTR *Soundcard,
            /* [out] */ BSTR *Port,
            /* [out] */ BSTR *File);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetOutput )( 
            ICall * This,
            /* [out] */ BSTR *Soundcard,
            /* [out] */ BSTR *Port,
            /* [out] */ BSTR *File);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetCaptureMic )( 
            ICall * This,
            /* [out] */ BSTR *Port,
            /* [out] */ BSTR *File);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SetInput )( 
            ICall * This,
            /* [in] */ BSTR Soundcard,
            /* [in] */ BSTR Port,
            /* [in] */ BSTR File);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SetOutput )( 
            ICall * This,
            /* [in] */ BSTR Soundcard,
            /* [in] */ BSTR Port,
            /* [in] */ BSTR File);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SetCaptureMic )( 
            ICall * This,
            /* [in] */ BSTR Port,
            /* [in] */ BSTR File);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VideoStatus )( 
            ICall * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *StartVideoSend )( 
            ICall * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *StopVideoSend )( 
            ICall * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *StartVideoRecieve )( 
            ICall * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *StopVideoRecieve )( 
            ICall * This);
        
        END_INTERFACE
    } ICallVtbl;

    interface ICall
    {
        CONST_VTBL struct ICallVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICall_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICall_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICall_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICall_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICall_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICall_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICall_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICall_get_ID(This,pVal)	\
    ( (This)->lpVtbl -> get_ID(This,pVal) ) 

#define ICall_get_Timestamp(This,pVal)	\
    ( (This)->lpVtbl -> get_Timestamp(This,pVal) ) 

#define ICall_get_PartnerHandle(This,pVal)	\
    ( (This)->lpVtbl -> get_PartnerHandle(This,pVal) ) 

#define ICall_get_PartnerDisplayName(This,pVal)	\
    ( (This)->lpVtbl -> get_PartnerDisplayName(This,pVal) ) 

#define ICall_get_ConferenceID(This,pVal)	\
    ( (This)->lpVtbl -> get_ConferenceID(This,pVal) ) 

#define ICall_get_Type(This,pVal)	\
    ( (This)->lpVtbl -> get_Type(This,pVal) ) 

#define ICall_get_Status(This,pVal)	\
    ( (This)->lpVtbl -> get_Status(This,pVal) ) 

#define ICall_put_Status(This,newVal)	\
    ( (This)->lpVtbl -> put_Status(This,newVal) ) 

#define ICall_get_FailureReason(This,pVal)	\
    ( (This)->lpVtbl -> get_FailureReason(This,pVal) ) 

#define ICall_get_Subject(This,pVal)	\
    ( (This)->lpVtbl -> get_Subject(This,pVal) ) 

#define ICall_get_PSTN_Number(This,pVal)	\
    ( (This)->lpVtbl -> get_PSTN_Number(This,pVal) ) 

#define ICall_get_Duration(This,pVal)	\
    ( (This)->lpVtbl -> get_Duration(This,pVal) ) 

#define ICall_get_PSTN_Status(This,pVal)	\
    ( (This)->lpVtbl -> get_PSTN_Status(This,pVal) ) 

#define ICall_get_ParticipantCount(This,pVal)	\
    ( (This)->lpVtbl -> get_ParticipantCount(This,pVal) ) 

#define ICall_GetParticipantInformation(This,Number,Handle,Type,Status,DisplayName)	\
    ( (This)->lpVtbl -> GetParticipantInformation(This,Number,Handle,Type,Status,DisplayName) ) 

#define ICall_JoinConference(This,CallID)	\
    ( (This)->lpVtbl -> JoinConference(This,CallID) ) 

#define ICall_get_VoiceMailDuration(This,pVal)	\
    ( (This)->lpVtbl -> get_VoiceMailDuration(This,pVal) ) 

#define ICall_get_VoiceMailAllowedDuration(This,pVal)	\
    ( (This)->lpVtbl -> get_VoiceMailAllowedDuration(This,pVal) ) 

#define ICall_SendDTMF(This,Codes)	\
    ( (This)->lpVtbl -> SendDTMF(This,Codes) ) 

#define ICall_GetInput(This,Soundcard,Port,File)	\
    ( (This)->lpVtbl -> GetInput(This,Soundcard,Port,File) ) 

#define ICall_GetOutput(This,Soundcard,Port,File)	\
    ( (This)->lpVtbl -> GetOutput(This,Soundcard,Port,File) ) 

#define ICall_GetCaptureMic(This,Port,File)	\
    ( (This)->lpVtbl -> GetCaptureMic(This,Port,File) ) 

#define ICall_SetInput(This,Soundcard,Port,File)	\
    ( (This)->lpVtbl -> SetInput(This,Soundcard,Port,File) ) 

#define ICall_SetOutput(This,Soundcard,Port,File)	\
    ( (This)->lpVtbl -> SetOutput(This,Soundcard,Port,File) ) 

#define ICall_SetCaptureMic(This,Port,File)	\
    ( (This)->lpVtbl -> SetCaptureMic(This,Port,File) ) 

#define ICall_get_VideoStatus(This,pVal)	\
    ( (This)->lpVtbl -> get_VideoStatus(This,pVal) ) 

#define ICall_StartVideoSend(This)	\
    ( (This)->lpVtbl -> StartVideoSend(This) ) 

#define ICall_StopVideoSend(This)	\
    ( (This)->lpVtbl -> StopVideoSend(This) ) 

#define ICall_StartVideoRecieve(This)	\
    ( (This)->lpVtbl -> StartVideoRecieve(This) ) 

#define ICall_StopVideoRecieve(This)	\
    ( (This)->lpVtbl -> StopVideoRecieve(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICall_INTERFACE_DEFINED__ */


#ifndef __IUserCollection_INTERFACE_DEFINED__
#define __IUserCollection_INTERFACE_DEFINED__

/* interface IUserCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUserCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5B0EB0B9-BF7C-4119-9886-568177FF1FF7")
    IUserCollection : public IDispatch
    {
    public:
        virtual /* [id][restricted][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppUnk) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUser **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUserCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUserCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUserCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUserCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUserCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUserCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUserCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUserCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IUserCollection * This,
            /* [retval][out] */ IUnknown **ppUnk);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IUserCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IUserCollection * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUser **pVal);
        
        END_INTERFACE
    } IUserCollectionVtbl;

    interface IUserCollection
    {
        CONST_VTBL struct IUserCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUserCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUserCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUserCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUserCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUserCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUserCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUserCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUserCollection_get__NewEnum(This,ppUnk)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppUnk) ) 

#define IUserCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IUserCollection_get_Item(This,index,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,index,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUserCollection_INTERFACE_DEFINED__ */


#ifndef __IUser_INTERFACE_DEFINED__
#define __IUser_INTERFACE_DEFINED__

/* interface IUser */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("43CCA21E-9F58-4298-B31E-464011A96637")
    IUser : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Handle( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_FullName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Birthdate( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Sex( 
            /* [retval][out] */ enum SkypeUserSex *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Country( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Province( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_City( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Phone_Home( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Phone_Office( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Phone_Mobile( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Homepage( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_About( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_HasCallEquipment( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_BuddyStatus( 
            /* [retval][out] */ enum SkypeBuddyStatus *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_BuddyStatus( 
            /* [in] */ enum SkypeBuddyStatus Val) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Authorized( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Blocked( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisplayName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_OnlineStatus( 
            /* [retval][out] */ enum SkypeOnlineStatus *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastOnline( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsSkypeOut( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ReceivedAuthorizationRequest( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsAuthorized( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsAuthorized( 
            /* [in] */ VARIANT_BOOL Val) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsBlocked( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsBlocked( 
            /* [in] */ VARIANT_BOOL Val) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SpeedDial( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_SpeedDial( 
            /* [in] */ BSTR Val) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MoodText( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Aliases( 
            /* [retval][out] */ SAFEARRAY * *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Timezone( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Language( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUser * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUser * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Handle )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FullName )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Birthdate )( 
            IUser * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Sex )( 
            IUser * This,
            /* [retval][out] */ enum SkypeUserSex *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Country )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Province )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_City )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Phone_Home )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Phone_Office )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Phone_Mobile )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Homepage )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_About )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HasCallEquipment )( 
            IUser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BuddyStatus )( 
            IUser * This,
            /* [retval][out] */ enum SkypeBuddyStatus *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BuddyStatus )( 
            IUser * This,
            /* [in] */ enum SkypeBuddyStatus Val);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Authorized )( 
            IUser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Blocked )( 
            IUser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayName )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OnlineStatus )( 
            IUser * This,
            /* [retval][out] */ enum SkypeOnlineStatus *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastOnline )( 
            IUser * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsSkypeOut )( 
            IUser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ReceivedAuthorizationRequest )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsAuthorized )( 
            IUser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsAuthorized )( 
            IUser * This,
            /* [in] */ VARIANT_BOOL Val);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsBlocked )( 
            IUser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsBlocked )( 
            IUser * This,
            /* [in] */ VARIANT_BOOL Val);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpeedDial )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpeedDial )( 
            IUser * This,
            /* [in] */ BSTR Val);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MoodText )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Aliases )( 
            IUser * This,
            /* [retval][out] */ SAFEARRAY * *pVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Timezone )( 
            IUser * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Language )( 
            IUser * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } IUserVtbl;

    interface IUser
    {
        CONST_VTBL struct IUserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUser_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUser_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUser_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUser_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUser_get_Handle(This,pVal)	\
    ( (This)->lpVtbl -> get_Handle(This,pVal) ) 

#define IUser_get_FullName(This,pVal)	\
    ( (This)->lpVtbl -> get_FullName(This,pVal) ) 

#define IUser_get_Birthdate(This,pVal)	\
    ( (This)->lpVtbl -> get_Birthdate(This,pVal) ) 

#define IUser_get_Sex(This,pVal)	\
    ( (This)->lpVtbl -> get_Sex(This,pVal) ) 

#define IUser_get_Country(This,pVal)	\
    ( (This)->lpVtbl -> get_Country(This,pVal) ) 

#define IUser_get_Province(This,pVal)	\
    ( (This)->lpVtbl -> get_Province(This,pVal) ) 

#define IUser_get_City(This,pVal)	\
    ( (This)->lpVtbl -> get_City(This,pVal) ) 

#define IUser_get_Phone_Home(This,pVal)	\
    ( (This)->lpVtbl -> get_Phone_Home(This,pVal) ) 

#define IUser_get_Phone_Office(This,pVal)	\
    ( (This)->lpVtbl -> get_Phone_Office(This,pVal) ) 

#define IUser_get_Phone_Mobile(This,pVal)	\
    ( (This)->lpVtbl -> get_Phone_Mobile(This,pVal) ) 

#define IUser_get_Homepage(This,pVal)	\
    ( (This)->lpVtbl -> get_Homepage(This,pVal) ) 

#define IUser_get_About(This,pVal)	\
    ( (This)->lpVtbl -> get_About(This,pVal) ) 

#define IUser_get_HasCallEquipment(This,pVal)	\
    ( (This)->lpVtbl -> get_HasCallEquipment(This,pVal) ) 

#define IUser_get_BuddyStatus(This,pVal)	\
    ( (This)->lpVtbl -> get_BuddyStatus(This,pVal) ) 

#define IUser_put_BuddyStatus(This,Val)	\
    ( (This)->lpVtbl -> put_BuddyStatus(This,Val) ) 

#define IUser_get_Authorized(This,pVal)	\
    ( (This)->lpVtbl -> get_Authorized(This,pVal) ) 

#define IUser_get_Blocked(This,pVal)	\
    ( (This)->lpVtbl -> get_Blocked(This,pVal) ) 

#define IUser_get_DisplayName(This,pVal)	\
    ( (This)->lpVtbl -> get_DisplayName(This,pVal) ) 

#define IUser_get_OnlineStatus(This,pVal)	\
    ( (This)->lpVtbl -> get_OnlineStatus(This,pVal) ) 

#define IUser_get_LastOnline(This,pVal)	\
    ( (This)->lpVtbl -> get_LastOnline(This,pVal) ) 

#define IUser_get_IsSkypeOut(This,pVal)	\
    ( (This)->lpVtbl -> get_IsSkypeOut(This,pVal) ) 

#define IUser_get_ReceivedAuthorizationRequest(This,pVal)	\
    ( (This)->lpVtbl -> get_ReceivedAuthorizationRequest(This,pVal) ) 

#define IUser_get_IsAuthorized(This,pVal)	\
    ( (This)->lpVtbl -> get_IsAuthorized(This,pVal) ) 

#define IUser_put_IsAuthorized(This,Val)	\
    ( (This)->lpVtbl -> put_IsAuthorized(This,Val) ) 

#define IUser_get_IsBlocked(This,pVal)	\
    ( (This)->lpVtbl -> get_IsBlocked(This,pVal) ) 

#define IUser_put_IsBlocked(This,Val)	\
    ( (This)->lpVtbl -> put_IsBlocked(This,Val) ) 

#define IUser_get_SpeedDial(This,pVal)	\
    ( (This)->lpVtbl -> get_SpeedDial(This,pVal) ) 

#define IUser_put_SpeedDial(This,Val)	\
    ( (This)->lpVtbl -> put_SpeedDial(This,Val) ) 

#define IUser_get_MoodText(This,pVal)	\
    ( (This)->lpVtbl -> get_MoodText(This,pVal) ) 

#define IUser_get_Aliases(This,pVal)	\
    ( (This)->lpVtbl -> get_Aliases(This,pVal) ) 

#define IUser_get_Timezone(This,pVal)	\
    ( (This)->lpVtbl -> get_Timezone(This,pVal) ) 

#define IUser_get_Language(This,pVal)	\
    ( (This)->lpVtbl -> get_Language(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUser_INTERFACE_DEFINED__ */


#ifndef __IApp2App_INTERFACE_DEFINED__
#define __IApp2App_INTERFACE_DEFINED__

/* interface IApp2App */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IApp2App;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E8EF86A8-27ED-4c9b-9096-E0B32005B9DB")
    IApp2App : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE CreateApplication( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE DeleteApplication( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE InitiateConnection( 
            /* [in] */ BSTR UserHandle) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ConnectableUsers( 
            /* [retval][out] */ IUserCollection **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IApp2AppVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IApp2App * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IApp2App * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IApp2App * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IApp2App * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IApp2App * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IApp2App * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IApp2App * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *CreateApplication )( 
            IApp2App * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteApplication )( 
            IApp2App * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *InitiateConnection )( 
            IApp2App * This,
            /* [in] */ BSTR UserHandle);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ConnectableUsers )( 
            IApp2App * This,
            /* [retval][out] */ IUserCollection **pVal);
        
        END_INTERFACE
    } IApp2AppVtbl;

    interface IApp2App
    {
        CONST_VTBL struct IApp2AppVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IApp2App_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IApp2App_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IApp2App_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IApp2App_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IApp2App_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IApp2App_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IApp2App_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IApp2App_CreateApplication(This)	\
    ( (This)->lpVtbl -> CreateApplication(This) ) 

#define IApp2App_DeleteApplication(This)	\
    ( (This)->lpVtbl -> DeleteApplication(This) ) 

#define IApp2App_InitiateConnection(This,UserHandle)	\
    ( (This)->lpVtbl -> InitiateConnection(This,UserHandle) ) 

#define IApp2App_get_ConnectableUsers(This,pVal)	\
    ( (This)->lpVtbl -> get_ConnectableUsers(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IApp2App_INTERFACE_DEFINED__ */


#ifndef __IApp2AppStream_INTERFACE_DEFINED__
#define __IApp2AppStream_INTERFACE_DEFINED__

/* interface IApp2AppStream */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IApp2AppStream;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F4E034C7-B5CD-4852-9A89-011F30CE42CE")
    IApp2AppStream : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE WriteData( 
            /* [in] */ BSTR TextData) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE WriteDatagram( 
            /* [in] */ BSTR TextData) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_StreamID( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_User( 
            /* [retval][out] */ IUser **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE WriteBinaryData( 
            /* [in] */ BSTR ID,
            /* [in] */ SAFEARRAY * *Array) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IApp2AppStreamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IApp2AppStream * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IApp2AppStream * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IApp2AppStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IApp2AppStream * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IApp2AppStream * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IApp2AppStream * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IApp2AppStream * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            IApp2AppStream * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *WriteData )( 
            IApp2AppStream * This,
            /* [in] */ BSTR TextData);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *WriteDatagram )( 
            IApp2AppStream * This,
            /* [in] */ BSTR TextData);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_StreamID )( 
            IApp2AppStream * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_User )( 
            IApp2AppStream * This,
            /* [retval][out] */ IUser **pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *WriteBinaryData )( 
            IApp2AppStream * This,
            /* [in] */ BSTR ID,
            /* [in] */ SAFEARRAY * *Array);
        
        END_INTERFACE
    } IApp2AppStreamVtbl;

    interface IApp2AppStream
    {
        CONST_VTBL struct IApp2AppStreamVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IApp2AppStream_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IApp2AppStream_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IApp2AppStream_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IApp2AppStream_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IApp2AppStream_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IApp2AppStream_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IApp2AppStream_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IApp2AppStream_Disconnect(This)	\
    ( (This)->lpVtbl -> Disconnect(This) ) 

#define IApp2AppStream_WriteData(This,TextData)	\
    ( (This)->lpVtbl -> WriteData(This,TextData) ) 

#define IApp2AppStream_WriteDatagram(This,TextData)	\
    ( (This)->lpVtbl -> WriteDatagram(This,TextData) ) 

#define IApp2AppStream_get_StreamID(This,pVal)	\
    ( (This)->lpVtbl -> get_StreamID(This,pVal) ) 

#define IApp2AppStream_get_User(This,pVal)	\
    ( (This)->lpVtbl -> get_User(This,pVal) ) 

#define IApp2AppStream_WriteBinaryData(This,ID,Array)	\
    ( (This)->lpVtbl -> WriteBinaryData(This,ID,Array) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IApp2AppStream_INTERFACE_DEFINED__ */


#ifndef __IMessageCollection_INTERFACE_DEFINED__
#define __IMessageCollection_INTERFACE_DEFINED__

/* interface IMessageCollection */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMessageCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("20E24A12-DCB4-47af-A59C-37D7BB13BCC0")
    IMessageCollection : public IDispatch
    {
    public:
        virtual /* [id][restricted][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppUnk) = 0;
        
        virtual /* [id][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IMessage **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMessageCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMessageCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMessageCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMessageCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMessageCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMessageCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMessageCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMessageCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][restricted][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IMessageCollection * This,
            /* [retval][out] */ IUnknown **ppUnk);
        
        /* [id][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IMessageCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [id][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IMessageCollection * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IMessage **pVal);
        
        END_INTERFACE
    } IMessageCollectionVtbl;

    interface IMessageCollection
    {
        CONST_VTBL struct IMessageCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMessageCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMessageCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMessageCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMessageCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMessageCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMessageCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMessageCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMessageCollection_get__NewEnum(This,ppUnk)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppUnk) ) 

#define IMessageCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IMessageCollection_get_Item(This,index,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,index,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMessageCollection_INTERFACE_DEFINED__ */


#ifndef __IMessage_INTERFACE_DEFINED__
#define __IMessage_INTERFACE_DEFINED__

/* interface IMessage */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMessage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B4E8AFEF-B5CE-4681-A0F5-53A32B168676")
    IMessage : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ enum SkypeMessageStatus *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Timestamp( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_PartnerHandle( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_PartnerDisplayName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ConferenceID( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ enum SkypeMessageType *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TypeAsText( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Body( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Seen( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Seen( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMessageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMessage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMessage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMessage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMessage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMessage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMessage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMessage * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ID )( 
            IMessage * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IMessage * This,
            /* [retval][out] */ enum SkypeMessageStatus *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Timestamp )( 
            IMessage * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PartnerHandle )( 
            IMessage * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PartnerDisplayName )( 
            IMessage * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ConferenceID )( 
            IMessage * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IMessage * This,
            /* [retval][out] */ enum SkypeMessageType *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TypeAsText )( 
            IMessage * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Body )( 
            IMessage * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Seen )( 
            IMessage * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Seen )( 
            IMessage * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } IMessageVtbl;

    interface IMessage
    {
        CONST_VTBL struct IMessageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMessage_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMessage_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMessage_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMessage_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMessage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMessage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMessage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMessage_get_ID(This,pVal)	\
    ( (This)->lpVtbl -> get_ID(This,pVal) ) 

#define IMessage_get_Status(This,pVal)	\
    ( (This)->lpVtbl -> get_Status(This,pVal) ) 

#define IMessage_get_Timestamp(This,pVal)	\
    ( (This)->lpVtbl -> get_Timestamp(This,pVal) ) 

#define IMessage_get_PartnerHandle(This,pVal)	\
    ( (This)->lpVtbl -> get_PartnerHandle(This,pVal) ) 

#define IMessage_get_PartnerDisplayName(This,pVal)	\
    ( (This)->lpVtbl -> get_PartnerDisplayName(This,pVal) ) 

#define IMessage_get_ConferenceID(This,pVal)	\
    ( (This)->lpVtbl -> get_ConferenceID(This,pVal) ) 

#define IMessage_get_Type(This,pVal)	\
    ( (This)->lpVtbl -> get_Type(This,pVal) ) 

#define IMessage_get_TypeAsText(This,pVal)	\
    ( (This)->lpVtbl -> get_TypeAsText(This,pVal) ) 

#define IMessage_get_Body(This,pVal)	\
    ( (This)->lpVtbl -> get_Body(This,pVal) ) 

#define IMessage_get_Seen(This,pVal)	\
    ( (This)->lpVtbl -> get_Seen(This,pVal) ) 

#define IMessage_put_Seen(This,newVal)	\
    ( (This)->lpVtbl -> put_Seen(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMessage_INTERFACE_DEFINED__ */


#ifndef __IChatMessageCollection_INTERFACE_DEFINED__
#define __IChatMessageCollection_INTERFACE_DEFINED__

/* interface IChatMessageCollection */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IChatMessageCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FBA3551A-6AF4-4812-9660-CBB6DF041EB7")
    IChatMessageCollection : public IDispatch
    {
    public:
        virtual /* [id][restricted][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppUnk) = 0;
        
        virtual /* [id][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IChatMessage **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IChatMessageCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IChatMessageCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IChatMessageCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IChatMessageCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IChatMessageCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IChatMessageCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IChatMessageCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IChatMessageCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][restricted][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IChatMessageCollection * This,
            /* [retval][out] */ IUnknown **ppUnk);
        
        /* [id][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IChatMessageCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [id][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IChatMessageCollection * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IChatMessage **pVal);
        
        END_INTERFACE
    } IChatMessageCollectionVtbl;

    interface IChatMessageCollection
    {
        CONST_VTBL struct IChatMessageCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IChatMessageCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IChatMessageCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IChatMessageCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IChatMessageCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IChatMessageCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IChatMessageCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IChatMessageCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IChatMessageCollection_get__NewEnum(This,ppUnk)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppUnk) ) 

#define IChatMessageCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IChatMessageCollection_get_Item(This,index,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,index,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IChatMessageCollection_INTERFACE_DEFINED__ */


#ifndef __IChatMessage_INTERFACE_DEFINED__
#define __IChatMessage_INTERFACE_DEFINED__

/* interface IChatMessage */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IChatMessage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5BF1A928-2A65-4482-892D-599047A57D2B")
    IChatMessage : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Chat( 
            /* [retval][out] */ IChat **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Timestamp( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_MessageFrom( 
            /* [retval][out] */ IUser **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_MessageType( 
            /* [retval][out] */ enum SkypeChatMessageType *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Users( 
            /* [retval][out] */ IUserCollection **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_LeftReason( 
            /* [retval][out] */ enum SkypeChatMessageLeftReason *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Body( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ enum SkypeChatMessageStatus *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Status( 
            /* [in] */ enum SkypeChatMessageStatus newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IChatMessageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IChatMessage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IChatMessage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IChatMessage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IChatMessage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IChatMessage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IChatMessage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IChatMessage * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ID )( 
            IChatMessage * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Chat )( 
            IChatMessage * This,
            /* [retval][out] */ IChat **pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Timestamp )( 
            IChatMessage * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MessageFrom )( 
            IChatMessage * This,
            /* [retval][out] */ IUser **pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MessageType )( 
            IChatMessage * This,
            /* [retval][out] */ enum SkypeChatMessageType *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Users )( 
            IChatMessage * This,
            /* [retval][out] */ IUserCollection **pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LeftReason )( 
            IChatMessage * This,
            /* [retval][out] */ enum SkypeChatMessageLeftReason *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Body )( 
            IChatMessage * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IChatMessage * This,
            /* [retval][out] */ enum SkypeChatMessageStatus *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Status )( 
            IChatMessage * This,
            /* [in] */ enum SkypeChatMessageStatus newVal);
        
        END_INTERFACE
    } IChatMessageVtbl;

    interface IChatMessage
    {
        CONST_VTBL struct IChatMessageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IChatMessage_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IChatMessage_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IChatMessage_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IChatMessage_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IChatMessage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IChatMessage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IChatMessage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IChatMessage_get_ID(This,pVal)	\
    ( (This)->lpVtbl -> get_ID(This,pVal) ) 

#define IChatMessage_get_Chat(This,pVal)	\
    ( (This)->lpVtbl -> get_Chat(This,pVal) ) 

#define IChatMessage_get_Timestamp(This,pVal)	\
    ( (This)->lpVtbl -> get_Timestamp(This,pVal) ) 

#define IChatMessage_get_MessageFrom(This,pVal)	\
    ( (This)->lpVtbl -> get_MessageFrom(This,pVal) ) 

#define IChatMessage_get_MessageType(This,pVal)	\
    ( (This)->lpVtbl -> get_MessageType(This,pVal) ) 

#define IChatMessage_get_Users(This,pVal)	\
    ( (This)->lpVtbl -> get_Users(This,pVal) ) 

#define IChatMessage_get_LeftReason(This,pVal)	\
    ( (This)->lpVtbl -> get_LeftReason(This,pVal) ) 

#define IChatMessage_get_Body(This,pVal)	\
    ( (This)->lpVtbl -> get_Body(This,pVal) ) 

#define IChatMessage_get_Status(This,pVal)	\
    ( (This)->lpVtbl -> get_Status(This,pVal) ) 

#define IChatMessage_put_Status(This,newVal)	\
    ( (This)->lpVtbl -> put_Status(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IChatMessage_INTERFACE_DEFINED__ */


#ifndef __IChatCollection_INTERFACE_DEFINED__
#define __IChatCollection_INTERFACE_DEFINED__

/* interface IChatCollection */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IChatCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("046D1967-46D1-4aad-A395-1AB1ADB8A15E")
    IChatCollection : public IDispatch
    {
    public:
        virtual /* [id][restricted][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppUnk) = 0;
        
        virtual /* [id][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IChat **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IChatCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IChatCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IChatCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IChatCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IChatCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IChatCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IChatCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IChatCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][restricted][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IChatCollection * This,
            /* [retval][out] */ IUnknown **ppUnk);
        
        /* [id][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IChatCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [id][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IChatCollection * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IChat **pVal);
        
        END_INTERFACE
    } IChatCollectionVtbl;

    interface IChatCollection
    {
        CONST_VTBL struct IChatCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IChatCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IChatCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IChatCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IChatCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IChatCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IChatCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IChatCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IChatCollection_get__NewEnum(This,ppUnk)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppUnk) ) 

#define IChatCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IChatCollection_get_Item(This,index,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,index,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IChatCollection_INTERFACE_DEFINED__ */


#ifndef __IChat_INTERFACE_DEFINED__
#define __IChat_INTERFACE_DEFINED__

/* interface IChat */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IChat;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F3600A6E-3B69-4871-9AF7-AF0881B3A760")
    IChat : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Timestamp( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Adder( 
            /* [retval][out] */ IUser **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Posters( 
            /* [retval][out] */ IUserCollection **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Members( 
            /* [retval][out] */ IUserCollection **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Topic( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Topic( 
            /* [in] */ BSTR Topic) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveMembers( 
            /* [retval][out] */ IUserCollection **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_FriendlyName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ enum SkypeChatStatus *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SendMessageA( 
            /* [in] */ BSTR Message) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Leave( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE AddMembers( 
            /* [in] */ SAFEARRAY * *UserHandles) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE LaunchChatWindow( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetChatMessageList( 
            /* [retval][out] */ IChatMessageCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetRecentChatMessageList( 
            /* [retval][out] */ IChatMessageCollection **List) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IChatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IChat * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IChat * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IChat * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IChat * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IChat * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IChat * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IChat * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IChat * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Timestamp )( 
            IChat * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Adder )( 
            IChat * This,
            /* [retval][out] */ IUser **pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Posters )( 
            IChat * This,
            /* [retval][out] */ IUserCollection **pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Members )( 
            IChat * This,
            /* [retval][out] */ IUserCollection **pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Topic )( 
            IChat * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Topic )( 
            IChat * This,
            /* [in] */ BSTR Topic);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveMembers )( 
            IChat * This,
            /* [retval][out] */ IUserCollection **pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FriendlyName )( 
            IChat * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IChat * This,
            /* [retval][out] */ enum SkypeChatStatus *pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SendMessageA )( 
            IChat * This,
            /* [in] */ BSTR Message);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *Leave )( 
            IChat * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *AddMembers )( 
            IChat * This,
            /* [in] */ SAFEARRAY * *UserHandles);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *LaunchChatWindow )( 
            IChat * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetChatMessageList )( 
            IChat * This,
            /* [retval][out] */ IChatMessageCollection **List);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetRecentChatMessageList )( 
            IChat * This,
            /* [retval][out] */ IChatMessageCollection **List);
        
        END_INTERFACE
    } IChatVtbl;

    interface IChat
    {
        CONST_VTBL struct IChatVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IChat_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IChat_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IChat_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IChat_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IChat_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IChat_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IChat_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IChat_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IChat_get_Timestamp(This,pVal)	\
    ( (This)->lpVtbl -> get_Timestamp(This,pVal) ) 

#define IChat_get_Adder(This,pVal)	\
    ( (This)->lpVtbl -> get_Adder(This,pVal) ) 

#define IChat_get_Posters(This,pVal)	\
    ( (This)->lpVtbl -> get_Posters(This,pVal) ) 

#define IChat_get_Members(This,pVal)	\
    ( (This)->lpVtbl -> get_Members(This,pVal) ) 

#define IChat_get_Topic(This,pVal)	\
    ( (This)->lpVtbl -> get_Topic(This,pVal) ) 

#define IChat_put_Topic(This,Topic)	\
    ( (This)->lpVtbl -> put_Topic(This,Topic) ) 

#define IChat_get_ActiveMembers(This,pVal)	\
    ( (This)->lpVtbl -> get_ActiveMembers(This,pVal) ) 

#define IChat_get_FriendlyName(This,pVal)	\
    ( (This)->lpVtbl -> get_FriendlyName(This,pVal) ) 

#define IChat_get_Status(This,pVal)	\
    ( (This)->lpVtbl -> get_Status(This,pVal) ) 

#define IChat_SendMessageA(This,Message)	\
    ( (This)->lpVtbl -> SendMessageA(This,Message) ) 

#define IChat_Leave(This)	\
    ( (This)->lpVtbl -> Leave(This) ) 

#define IChat_AddMembers(This,UserHandles)	\
    ( (This)->lpVtbl -> AddMembers(This,UserHandles) ) 

#define IChat_LaunchChatWindow(This)	\
    ( (This)->lpVtbl -> LaunchChatWindow(This) ) 

#define IChat_GetChatMessageList(This,List)	\
    ( (This)->lpVtbl -> GetChatMessageList(This,List) ) 

#define IChat_GetRecentChatMessageList(This,List)	\
    ( (This)->lpVtbl -> GetRecentChatMessageList(This,List) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IChat_INTERFACE_DEFINED__ */


#ifndef __IVoiceMailCollection_INTERFACE_DEFINED__
#define __IVoiceMailCollection_INTERFACE_DEFINED__

/* interface IVoiceMailCollection */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoiceMailCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("84228251-93C4-4132-A262-ECEA129FA6A8")
    IVoiceMailCollection : public IDispatch
    {
    public:
        virtual /* [id][restricted][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppUnk) = 0;
        
        virtual /* [id][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][helpcontext][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IVoiceMail **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVoiceMailCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoiceMailCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoiceMailCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoiceMailCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoiceMailCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoiceMailCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoiceMailCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoiceMailCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][restricted][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IVoiceMailCollection * This,
            /* [retval][out] */ IUnknown **ppUnk);
        
        /* [id][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IVoiceMailCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [id][helpcontext][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IVoiceMailCollection * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IVoiceMail **pVal);
        
        END_INTERFACE
    } IVoiceMailCollectionVtbl;

    interface IVoiceMailCollection
    {
        CONST_VTBL struct IVoiceMailCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoiceMailCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoiceMailCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoiceMailCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoiceMailCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoiceMailCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoiceMailCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoiceMailCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoiceMailCollection_get__NewEnum(This,ppUnk)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppUnk) ) 

#define IVoiceMailCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IVoiceMailCollection_get_Item(This,index,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,index,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IVoiceMailCollection_INTERFACE_DEFINED__ */


#ifndef __IVoiceMail_INTERFACE_DEFINED__
#define __IVoiceMail_INTERFACE_DEFINED__

/* interface IVoiceMail */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IVoiceMail;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5A45154B-09C9-4708-82F6-8D70F0FA6BC6")
    IVoiceMail : public IDispatch
    {
    public:
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ enum SkypeVoiceMailType *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PartnerHandle( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PartnerDisplayName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ enum SkypeVoiceMailStatus *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FailureReason( 
            /* [retval][out] */ enum SkypeVoiceMailFailure *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Subject( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Timestamp( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Duration( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AllowedDuration( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenAndPlay( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVoiceMailVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVoiceMail * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVoiceMail * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVoiceMail * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVoiceMail * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVoiceMail * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVoiceMail * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVoiceMail * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ID )( 
            IVoiceMail * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IVoiceMail * This,
            /* [retval][out] */ enum SkypeVoiceMailType *pVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PartnerHandle )( 
            IVoiceMail * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PartnerDisplayName )( 
            IVoiceMail * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IVoiceMail * This,
            /* [retval][out] */ enum SkypeVoiceMailStatus *pVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FailureReason )( 
            IVoiceMail * This,
            /* [retval][out] */ enum SkypeVoiceMailFailure *pVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Subject )( 
            IVoiceMail * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Timestamp )( 
            IVoiceMail * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Duration )( 
            IVoiceMail * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AllowedDuration )( 
            IVoiceMail * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenAndPlay )( 
            IVoiceMail * This);
        
        END_INTERFACE
    } IVoiceMailVtbl;

    interface IVoiceMail
    {
        CONST_VTBL struct IVoiceMailVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVoiceMail_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVoiceMail_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVoiceMail_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVoiceMail_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVoiceMail_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVoiceMail_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVoiceMail_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVoiceMail_get_ID(This,pVal)	\
    ( (This)->lpVtbl -> get_ID(This,pVal) ) 

#define IVoiceMail_get_Type(This,pVal)	\
    ( (This)->lpVtbl -> get_Type(This,pVal) ) 

#define IVoiceMail_get_PartnerHandle(This,pVal)	\
    ( (This)->lpVtbl -> get_PartnerHandle(This,pVal) ) 

#define IVoiceMail_get_PartnerDisplayName(This,pVal)	\
    ( (This)->lpVtbl -> get_PartnerDisplayName(This,pVal) ) 

#define IVoiceMail_get_Status(This,pVal)	\
    ( (This)->lpVtbl -> get_Status(This,pVal) ) 

#define IVoiceMail_get_FailureReason(This,pVal)	\
    ( (This)->lpVtbl -> get_FailureReason(This,pVal) ) 

#define IVoiceMail_get_Subject(This,pVal)	\
    ( (This)->lpVtbl -> get_Subject(This,pVal) ) 

#define IVoiceMail_get_Timestamp(This,pVal)	\
    ( (This)->lpVtbl -> get_Timestamp(This,pVal) ) 

#define IVoiceMail_get_Duration(This,pVal)	\
    ( (This)->lpVtbl -> get_Duration(This,pVal) ) 

#define IVoiceMail_get_AllowedDuration(This,pVal)	\
    ( (This)->lpVtbl -> get_AllowedDuration(This,pVal) ) 

#define IVoiceMail_OpenAndPlay(This)	\
    ( (This)->lpVtbl -> OpenAndPlay(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IVoiceMail_INTERFACE_DEFINED__ */


#ifndef __IUI_INTERFACE_DEFINED__
#define __IUI_INTERFACE_DEFINED__

/* interface IUI */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUI;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BE2A8840-5A65-49da-8DDF-F6A7C6D781E5")
    IUI : public IDispatch
    {
    public:
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenProfile( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenUserInfo( 
            /* [in] */ BSTR UserName) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenConference( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenSearch( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenOptions( 
            /* [in] */ enum SkypeOptionsPage Page) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenCallHistory( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenContacts( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenDialpad( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenSendContacts( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenBlockedUsers( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenImportContacts( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenGettingStarted( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenAuthorization( 
            /* [in] */ BSTR UserName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUIVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUI * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUI * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUI * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUI * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUI * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUI * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUI * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenProfile )( 
            IUI * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenUserInfo )( 
            IUI * This,
            /* [in] */ BSTR UserName);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenConference )( 
            IUI * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenSearch )( 
            IUI * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenOptions )( 
            IUI * This,
            /* [in] */ enum SkypeOptionsPage Page);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenCallHistory )( 
            IUI * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenContacts )( 
            IUI * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenDialpad )( 
            IUI * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenSendContacts )( 
            IUI * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenBlockedUsers )( 
            IUI * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenImportContacts )( 
            IUI * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenGettingStarted )( 
            IUI * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenAuthorization )( 
            IUI * This,
            /* [in] */ BSTR UserName);
        
        END_INTERFACE
    } IUIVtbl;

    interface IUI
    {
        CONST_VTBL struct IUIVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUI_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUI_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUI_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUI_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUI_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUI_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUI_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUI_OpenProfile(This)	\
    ( (This)->lpVtbl -> OpenProfile(This) ) 

#define IUI_OpenUserInfo(This,UserName)	\
    ( (This)->lpVtbl -> OpenUserInfo(This,UserName) ) 

#define IUI_OpenConference(This)	\
    ( (This)->lpVtbl -> OpenConference(This) ) 

#define IUI_OpenSearch(This)	\
    ( (This)->lpVtbl -> OpenSearch(This) ) 

#define IUI_OpenOptions(This,Page)	\
    ( (This)->lpVtbl -> OpenOptions(This,Page) ) 

#define IUI_OpenCallHistory(This)	\
    ( (This)->lpVtbl -> OpenCallHistory(This) ) 

#define IUI_OpenContacts(This)	\
    ( (This)->lpVtbl -> OpenContacts(This) ) 

#define IUI_OpenDialpad(This)	\
    ( (This)->lpVtbl -> OpenDialpad(This) ) 

#define IUI_OpenSendContacts(This)	\
    ( (This)->lpVtbl -> OpenSendContacts(This) ) 

#define IUI_OpenBlockedUsers(This)	\
    ( (This)->lpVtbl -> OpenBlockedUsers(This) ) 

#define IUI_OpenImportContacts(This)	\
    ( (This)->lpVtbl -> OpenImportContacts(This) ) 

#define IUI_OpenGettingStarted(This)	\
    ( (This)->lpVtbl -> OpenGettingStarted(This) ) 

#define IUI_OpenAuthorization(This,UserName)	\
    ( (This)->lpVtbl -> OpenAuthorization(This,UserName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUI_INTERFACE_DEFINED__ */


/* interface __MIDL_itf__SkypeAPI_0000_0016 */
/* [local] */ 

/* [uuid] */ struct  DECLSPEC_UUID("1F2CC8D7-37CA-4e76-88DD-F79486265F59") ForwardingRule
    {
    LONG StartTime;
    LONG EndTime;
    BSTR Handle;
    } ;


extern RPC_IF_HANDLE __MIDL_itf__SkypeAPI_0000_0016_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf__SkypeAPI_0000_0016_v0_0_s_ifspec;

#ifndef __IProfile_INTERFACE_DEFINED__
#define __IProfile_INTERFACE_DEFINED__

/* interface IProfile */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IProfile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5106D5C0-C57A-4ac7-A361-ECC530A55BDE")
    IProfile : public IDispatch
    {
    public:
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FullName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FullName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Birthday( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Birthday( 
            /* [in] */ DATE newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Sex( 
            /* [retval][out] */ enum SkypeUserSex *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Sex( 
            /* [in] */ enum SkypeUserSex newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Languages( 
            /* [retval][out] */ SAFEARRAY * *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Languages( 
            /* [in] */ SAFEARRAY * newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Country( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Country( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Province( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Province( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_City( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_City( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HomePhone( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_HomePhone( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_OfficePhone( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_OfficePhone( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MobilePhone( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MobilePhone( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Homepage( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Homepage( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_About( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_About( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MoodText( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MoodText( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Timezone( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Timezone( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ForwardingRules( 
            /* [retval][out] */ SAFEARRAY * *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ForwardingRules( 
            /* [in] */ SAFEARRAY * newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NoAnswerTimeout( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NoAnswerTimeout( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CallForwardingEnabled( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CallForwardingEnabled( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VoicemailEnabled( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpcontext][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_VoicemailEnabled( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_RingtoneStatus( 
            /* [in] */ enum SkypeRingtone Tone,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_RingtoneFileName( 
            /* [in] */ enum SkypeRingtone Tone,
            /* [retval][out] */ BSTR *FileName) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_RingtoneFileName( 
            /* [in] */ enum SkypeRingtone Tone,
            /* [in] */ BSTR FileName) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_AvatarFileName( 
            /* [in] */ LONG Index,
            /* [in] */ BSTR FileName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProfileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProfile * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProfile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProfile * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IProfile * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IProfile * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IProfile * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IProfile * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FullName )( 
            IProfile * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FullName )( 
            IProfile * This,
            /* [in] */ BSTR newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Birthday )( 
            IProfile * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Birthday )( 
            IProfile * This,
            /* [in] */ DATE newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Sex )( 
            IProfile * This,
            /* [retval][out] */ enum SkypeUserSex *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Sex )( 
            IProfile * This,
            /* [in] */ enum SkypeUserSex newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Languages )( 
            IProfile * This,
            /* [retval][out] */ SAFEARRAY * *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Languages )( 
            IProfile * This,
            /* [in] */ SAFEARRAY * newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Country )( 
            IProfile * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Country )( 
            IProfile * This,
            /* [in] */ BSTR newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Province )( 
            IProfile * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Province )( 
            IProfile * This,
            /* [in] */ BSTR newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_City )( 
            IProfile * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_City )( 
            IProfile * This,
            /* [in] */ BSTR newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HomePhone )( 
            IProfile * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HomePhone )( 
            IProfile * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OfficePhone )( 
            IProfile * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_OfficePhone )( 
            IProfile * This,
            /* [in] */ BSTR newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MobilePhone )( 
            IProfile * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MobilePhone )( 
            IProfile * This,
            /* [in] */ BSTR newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Homepage )( 
            IProfile * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Homepage )( 
            IProfile * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_About )( 
            IProfile * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_About )( 
            IProfile * This,
            /* [in] */ BSTR newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MoodText )( 
            IProfile * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MoodText )( 
            IProfile * This,
            /* [in] */ BSTR newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Timezone )( 
            IProfile * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Timezone )( 
            IProfile * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ForwardingRules )( 
            IProfile * This,
            /* [retval][out] */ SAFEARRAY * *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ForwardingRules )( 
            IProfile * This,
            /* [in] */ SAFEARRAY * newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NoAnswerTimeout )( 
            IProfile * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NoAnswerTimeout )( 
            IProfile * This,
            /* [in] */ LONG newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CallForwardingEnabled )( 
            IProfile * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CallForwardingEnabled )( 
            IProfile * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpcontext][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VoicemailEnabled )( 
            IProfile * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpcontext][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_VoicemailEnabled )( 
            IProfile * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RingtoneStatus )( 
            IProfile * This,
            /* [in] */ enum SkypeRingtone Tone,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RingtoneFileName )( 
            IProfile * This,
            /* [in] */ enum SkypeRingtone Tone,
            /* [retval][out] */ BSTR *FileName);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RingtoneFileName )( 
            IProfile * This,
            /* [in] */ enum SkypeRingtone Tone,
            /* [in] */ BSTR FileName);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AvatarFileName )( 
            IProfile * This,
            /* [in] */ LONG Index,
            /* [in] */ BSTR FileName);
        
        END_INTERFACE
    } IProfileVtbl;

    interface IProfile
    {
        CONST_VTBL struct IProfileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProfile_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IProfile_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IProfile_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IProfile_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IProfile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IProfile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IProfile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IProfile_get_FullName(This,pVal)	\
    ( (This)->lpVtbl -> get_FullName(This,pVal) ) 

#define IProfile_put_FullName(This,newVal)	\
    ( (This)->lpVtbl -> put_FullName(This,newVal) ) 

#define IProfile_get_Birthday(This,pVal)	\
    ( (This)->lpVtbl -> get_Birthday(This,pVal) ) 

#define IProfile_put_Birthday(This,newVal)	\
    ( (This)->lpVtbl -> put_Birthday(This,newVal) ) 

#define IProfile_get_Sex(This,pVal)	\
    ( (This)->lpVtbl -> get_Sex(This,pVal) ) 

#define IProfile_put_Sex(This,newVal)	\
    ( (This)->lpVtbl -> put_Sex(This,newVal) ) 

#define IProfile_get_Languages(This,pVal)	\
    ( (This)->lpVtbl -> get_Languages(This,pVal) ) 

#define IProfile_put_Languages(This,newVal)	\
    ( (This)->lpVtbl -> put_Languages(This,newVal) ) 

#define IProfile_get_Country(This,pVal)	\
    ( (This)->lpVtbl -> get_Country(This,pVal) ) 

#define IProfile_put_Country(This,newVal)	\
    ( (This)->lpVtbl -> put_Country(This,newVal) ) 

#define IProfile_get_Province(This,pVal)	\
    ( (This)->lpVtbl -> get_Province(This,pVal) ) 

#define IProfile_put_Province(This,newVal)	\
    ( (This)->lpVtbl -> put_Province(This,newVal) ) 

#define IProfile_get_City(This,pVal)	\
    ( (This)->lpVtbl -> get_City(This,pVal) ) 

#define IProfile_put_City(This,newVal)	\
    ( (This)->lpVtbl -> put_City(This,newVal) ) 

#define IProfile_get_HomePhone(This,pVal)	\
    ( (This)->lpVtbl -> get_HomePhone(This,pVal) ) 

#define IProfile_put_HomePhone(This,newVal)	\
    ( (This)->lpVtbl -> put_HomePhone(This,newVal) ) 

#define IProfile_get_OfficePhone(This,pVal)	\
    ( (This)->lpVtbl -> get_OfficePhone(This,pVal) ) 

#define IProfile_put_OfficePhone(This,newVal)	\
    ( (This)->lpVtbl -> put_OfficePhone(This,newVal) ) 

#define IProfile_get_MobilePhone(This,pVal)	\
    ( (This)->lpVtbl -> get_MobilePhone(This,pVal) ) 

#define IProfile_put_MobilePhone(This,newVal)	\
    ( (This)->lpVtbl -> put_MobilePhone(This,newVal) ) 

#define IProfile_get_Homepage(This,pVal)	\
    ( (This)->lpVtbl -> get_Homepage(This,pVal) ) 

#define IProfile_put_Homepage(This,newVal)	\
    ( (This)->lpVtbl -> put_Homepage(This,newVal) ) 

#define IProfile_get_About(This,pVal)	\
    ( (This)->lpVtbl -> get_About(This,pVal) ) 

#define IProfile_put_About(This,newVal)	\
    ( (This)->lpVtbl -> put_About(This,newVal) ) 

#define IProfile_get_MoodText(This,pVal)	\
    ( (This)->lpVtbl -> get_MoodText(This,pVal) ) 

#define IProfile_put_MoodText(This,newVal)	\
    ( (This)->lpVtbl -> put_MoodText(This,newVal) ) 

#define IProfile_get_Timezone(This,pVal)	\
    ( (This)->lpVtbl -> get_Timezone(This,pVal) ) 

#define IProfile_put_Timezone(This,newVal)	\
    ( (This)->lpVtbl -> put_Timezone(This,newVal) ) 

#define IProfile_get_ForwardingRules(This,pVal)	\
    ( (This)->lpVtbl -> get_ForwardingRules(This,pVal) ) 

#define IProfile_put_ForwardingRules(This,newVal)	\
    ( (This)->lpVtbl -> put_ForwardingRules(This,newVal) ) 

#define IProfile_get_NoAnswerTimeout(This,pVal)	\
    ( (This)->lpVtbl -> get_NoAnswerTimeout(This,pVal) ) 

#define IProfile_put_NoAnswerTimeout(This,newVal)	\
    ( (This)->lpVtbl -> put_NoAnswerTimeout(This,newVal) ) 

#define IProfile_get_CallForwardingEnabled(This,pVal)	\
    ( (This)->lpVtbl -> get_CallForwardingEnabled(This,pVal) ) 

#define IProfile_put_CallForwardingEnabled(This,newVal)	\
    ( (This)->lpVtbl -> put_CallForwardingEnabled(This,newVal) ) 

#define IProfile_get_VoicemailEnabled(This,pVal)	\
    ( (This)->lpVtbl -> get_VoicemailEnabled(This,pVal) ) 

#define IProfile_put_VoicemailEnabled(This,newVal)	\
    ( (This)->lpVtbl -> put_VoicemailEnabled(This,newVal) ) 

#define IProfile_get_RingtoneStatus(This,Tone,pVal)	\
    ( (This)->lpVtbl -> get_RingtoneStatus(This,Tone,pVal) ) 

#define IProfile_get_RingtoneFileName(This,Tone,FileName)	\
    ( (This)->lpVtbl -> get_RingtoneFileName(This,Tone,FileName) ) 

#define IProfile_put_RingtoneFileName(This,Tone,FileName)	\
    ( (This)->lpVtbl -> put_RingtoneFileName(This,Tone,FileName) ) 

#define IProfile_put_AvatarFileName(This,Index,FileName)	\
    ( (This)->lpVtbl -> put_AvatarFileName(This,Index,FileName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IProfile_INTERFACE_DEFINED__ */


#ifndef __IAccess_INTERFACE_DEFINED__
#define __IAccess_INTERFACE_DEFINED__

/* interface IAccess */
/* [unique][helpcontext][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAccess;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56559E11-B2E0-4251-991D-9DA23BFC1C18")
    IAccess : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SendCommand( 
            /* [in] */ BSTR Command) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetCall( 
            /* [in] */ LONG CallID,
            /* [retval][out] */ ICall **Call) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE PlaceCall( 
            /* [in] */ BSTR UserName,
            /* [retval][out] */ ICall **Call) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SearchForUser( 
            /* [in] */ BSTR UserName,
            /* [retval][out] */ IUserCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetCallList( 
            /* [retval][out] */ ICallCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetFriendList( 
            /* [retval][out] */ IUserCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ConnectionStatus( 
            /* [retval][out] */ enum SkypeConnectionStatus *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_AudioIn( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_AudioIn( 
            /* [in] */ BSTR Val) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_AudioOut( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_AudioOut( 
            /* [in] */ BSTR Val) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentUserHandle( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentUserStatus( 
            /* [retval][out] */ enum SkypeOnlineStatus *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurrentUserStatus( 
            /* [in] */ enum SkypeOnlineStatus newVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Mute( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Mute( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetMessageA( 
            /* [in] */ LONG MessageID,
            /* [retval][out] */ IMessage **Message) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SendMessageA( 
            /* [in] */ BSTR UserHandle,
            /* [in] */ BSTR Text,
            /* [retval][out] */ IMessage **Message) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Connect( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetActiveCallList( 
            /* [retval][out] */ ICallCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetMissedCallList( 
            /* [retval][out] */ ICallCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetMessageList( 
            /* [retval][out] */ IMessageCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetMissedMessageList( 
            /* [retval][out] */ IMessageCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SearchForMessages( 
            /* [in] */ BSTR UserHandle,
            /* [retval][out] */ IMessageCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Privilege( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetUser( 
            /* [in] */ BSTR Handle,
            /* [retval][out] */ IUser **User) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SkypeExecutablePath( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Protocol( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Protocol( 
            /* [in] */ long lngVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetChatList( 
            /* [retval][out] */ IChatCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetChat( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ IChat **Chat) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetChatMessageList( 
            /* [retval][out] */ IChatMessageCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetMissedChatMessageList( 
            /* [retval][out] */ IChatMessageCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetChatMessage( 
            /* [in] */ LONG ID,
            /* [retval][out] */ IChatMessage **Message) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE CloseSkype( void) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SkypeOutBalance( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SkypeOutCurrency( 
            /* [retval][out] */ enum SkypeCurrencyType *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_APIAttachmentStatus( 
            /* [retval][out] */ enum SkypeAPIAttachmentStatus *APIStatus) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE PlaceConferenceCall( 
            /* [in] */ SAFEARRAY * *UserHandles,
            /* [retval][out] */ ICall **Call) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE LaunchChat( 
            /* [in] */ SAFEARRAY * *UserHandles,
            /* [retval][out] */ IChat **Chat) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE OpenIMDialog( 
            /* [in] */ BSTR UserHandle,
            /* [in] */ BSTR Message) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE FocusSkypeWindow( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE LaunchAddAFriend( 
            /* [in] */ BSTR UserHandle) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Conversion( 
            /* [retval][out] */ IConversion **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE MinimizeSkype( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SearchForChatMessages( 
            /* [in] */ BSTR UserHandle,
            /* [retval][out] */ IChatMessageCollection **List) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetActiveChatList( 
            /* [retval][out] */ IChatCollection **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetMissedChatList( 
            /* [retval][out] */ IChatCollection **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetRecentChatList( 
            /* [retval][out] */ IChatCollection **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetBookmarkedChatList( 
            /* [retval][out] */ IChatCollection **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetVoiceMailList( 
            /* [retval][out] */ IVoiceMailCollection **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SendVoiceMail( 
            /* [in] */ BSTR UserHandle,
            /* [retval][out] */ IVoiceMail **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetUsersWaitingForAuthorization( 
            /* [retval][out] */ IUserCollection **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ClearChatHistory( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ClearVoiceMailHistory( void) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ClearCallHistory( 
            /* [in] */ enum SkypeClearCallHistory CallType) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Ringer( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Ringer( 
            /* [in] */ BSTR Val) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ConnectAndWait( 
            /* [in] */ LONG TimeoutInSeconds) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE OpenFileTransferDialog( 
            /* [in] */ SAFEARRAY * *UserHandles,
            /* [in] */ BSTR Folder) = 0;
        
        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SendBlockingCommand( 
            /* [in] */ BSTR Command,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_CommandTimeout( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_CommandTimeout( 
            /* [in] */ LONG Val) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenApplication( 
            /* [in] */ BSTR ApplicationName,
            /* [retval][out] */ IApp2App **App) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Profile( 
            /* [retval][out] */ IProfile **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_UI( 
            /* [retval][out] */ IUI **pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_VideoIn( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_VideoIn( 
            /* [in] */ BSTR Val) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAccessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAccess * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAccess * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAccess * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAccess * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAccess * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAccess * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAccess * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SendCommand )( 
            IAccess * This,
            /* [in] */ BSTR Command);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetCall )( 
            IAccess * This,
            /* [in] */ LONG CallID,
            /* [retval][out] */ ICall **Call);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *PlaceCall )( 
            IAccess * This,
            /* [in] */ BSTR UserName,
            /* [retval][out] */ ICall **Call);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SearchForUser )( 
            IAccess * This,
            /* [in] */ BSTR UserName,
            /* [retval][out] */ IUserCollection **List);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetCallList )( 
            IAccess * This,
            /* [retval][out] */ ICallCollection **List);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetFriendList )( 
            IAccess * This,
            /* [retval][out] */ IUserCollection **List);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ConnectionStatus )( 
            IAccess * This,
            /* [retval][out] */ enum SkypeConnectionStatus *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioIn )( 
            IAccess * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AudioIn )( 
            IAccess * This,
            /* [in] */ BSTR Val);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioOut )( 
            IAccess * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AudioOut )( 
            IAccess * This,
            /* [in] */ BSTR Val);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentUserHandle )( 
            IAccess * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentUserStatus )( 
            IAccess * This,
            /* [retval][out] */ enum SkypeOnlineStatus *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurrentUserStatus )( 
            IAccess * This,
            /* [in] */ enum SkypeOnlineStatus newVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Mute )( 
            IAccess * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Mute )( 
            IAccess * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetMessageA )( 
            IAccess * This,
            /* [in] */ LONG MessageID,
            /* [retval][out] */ IMessage **Message);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SendMessageA )( 
            IAccess * This,
            /* [in] */ BSTR UserHandle,
            /* [in] */ BSTR Text,
            /* [retval][out] */ IMessage **Message);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            IAccess * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetActiveCallList )( 
            IAccess * This,
            /* [retval][out] */ ICallCollection **List);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetMissedCallList )( 
            IAccess * This,
            /* [retval][out] */ ICallCollection **List);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetMessageList )( 
            IAccess * This,
            /* [retval][out] */ IMessageCollection **List);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetMissedMessageList )( 
            IAccess * This,
            /* [retval][out] */ IMessageCollection **List);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SearchForMessages )( 
            IAccess * This,
            /* [in] */ BSTR UserHandle,
            /* [retval][out] */ IMessageCollection **List);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IAccess * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Privilege )( 
            IAccess * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetUser )( 
            IAccess * This,
            /* [in] */ BSTR Handle,
            /* [retval][out] */ IUser **User);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SkypeExecutablePath )( 
            IAccess * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Protocol )( 
            IAccess * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Protocol )( 
            IAccess * This,
            /* [in] */ long lngVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetChatList )( 
            IAccess * This,
            /* [retval][out] */ IChatCollection **List);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetChat )( 
            IAccess * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ IChat **Chat);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetChatMessageList )( 
            IAccess * This,
            /* [retval][out] */ IChatMessageCollection **List);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetMissedChatMessageList )( 
            IAccess * This,
            /* [retval][out] */ IChatMessageCollection **List);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetChatMessage )( 
            IAccess * This,
            /* [in] */ LONG ID,
            /* [retval][out] */ IChatMessage **Message);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *CloseSkype )( 
            IAccess * This);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SkypeOutBalance )( 
            IAccess * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SkypeOutCurrency )( 
            IAccess * This,
            /* [retval][out] */ enum SkypeCurrencyType *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_APIAttachmentStatus )( 
            IAccess * This,
            /* [retval][out] */ enum SkypeAPIAttachmentStatus *APIStatus);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *PlaceConferenceCall )( 
            IAccess * This,
            /* [in] */ SAFEARRAY * *UserHandles,
            /* [retval][out] */ ICall **Call);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *LaunchChat )( 
            IAccess * This,
            /* [in] */ SAFEARRAY * *UserHandles,
            /* [retval][out] */ IChat **Chat);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *OpenIMDialog )( 
            IAccess * This,
            /* [in] */ BSTR UserHandle,
            /* [in] */ BSTR Message);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *FocusSkypeWindow )( 
            IAccess * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *LaunchAddAFriend )( 
            IAccess * This,
            /* [in] */ BSTR UserHandle);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Conversion )( 
            IAccess * This,
            /* [retval][out] */ IConversion **pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *MinimizeSkype )( 
            IAccess * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SearchForChatMessages )( 
            IAccess * This,
            /* [in] */ BSTR UserHandle,
            /* [retval][out] */ IChatMessageCollection **List);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetActiveChatList )( 
            IAccess * This,
            /* [retval][out] */ IChatCollection **pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetMissedChatList )( 
            IAccess * This,
            /* [retval][out] */ IChatCollection **pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetRecentChatList )( 
            IAccess * This,
            /* [retval][out] */ IChatCollection **pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetBookmarkedChatList )( 
            IAccess * This,
            /* [retval][out] */ IChatCollection **pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetVoiceMailList )( 
            IAccess * This,
            /* [retval][out] */ IVoiceMailCollection **pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SendVoiceMail )( 
            IAccess * This,
            /* [in] */ BSTR UserHandle,
            /* [retval][out] */ IVoiceMail **pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetUsersWaitingForAuthorization )( 
            IAccess * This,
            /* [retval][out] */ IUserCollection **pVal);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *ClearChatHistory )( 
            IAccess * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *ClearVoiceMailHistory )( 
            IAccess * This);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *ClearCallHistory )( 
            IAccess * This,
            /* [in] */ enum SkypeClearCallHistory CallType);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Ringer )( 
            IAccess * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Ringer )( 
            IAccess * This,
            /* [in] */ BSTR Val);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectAndWait )( 
            IAccess * This,
            /* [in] */ LONG TimeoutInSeconds);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *OpenFileTransferDialog )( 
            IAccess * This,
            /* [in] */ SAFEARRAY * *UserHandles,
            /* [in] */ BSTR Folder);
        
        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *SendBlockingCommand )( 
            IAccess * This,
            /* [in] */ BSTR Command,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CommandTimeout )( 
            IAccess * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CommandTimeout )( 
            IAccess * This,
            /* [in] */ LONG Val);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenApplication )( 
            IAccess * This,
            /* [in] */ BSTR ApplicationName,
            /* [retval][out] */ IApp2App **App);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Profile )( 
            IAccess * This,
            /* [retval][out] */ IProfile **pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UI )( 
            IAccess * This,
            /* [retval][out] */ IUI **pVal);
        
        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VideoIn )( 
            IAccess * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_VideoIn )( 
            IAccess * This,
            /* [in] */ BSTR Val);
        
        END_INTERFACE
    } IAccessVtbl;

    interface IAccess
    {
        CONST_VTBL struct IAccessVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAccess_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAccess_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAccess_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAccess_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAccess_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAccess_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAccess_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IAccess_SendCommand(This,Command)	\
    ( (This)->lpVtbl -> SendCommand(This,Command) ) 

#define IAccess_GetCall(This,CallID,Call)	\
    ( (This)->lpVtbl -> GetCall(This,CallID,Call) ) 

#define IAccess_PlaceCall(This,UserName,Call)	\
    ( (This)->lpVtbl -> PlaceCall(This,UserName,Call) ) 

#define IAccess_SearchForUser(This,UserName,List)	\
    ( (This)->lpVtbl -> SearchForUser(This,UserName,List) ) 

#define IAccess_GetCallList(This,List)	\
    ( (This)->lpVtbl -> GetCallList(This,List) ) 

#define IAccess_GetFriendList(This,List)	\
    ( (This)->lpVtbl -> GetFriendList(This,List) ) 

#define IAccess_get_ConnectionStatus(This,pVal)	\
    ( (This)->lpVtbl -> get_ConnectionStatus(This,pVal) ) 

#define IAccess_get_AudioIn(This,pVal)	\
    ( (This)->lpVtbl -> get_AudioIn(This,pVal) ) 

#define IAccess_put_AudioIn(This,Val)	\
    ( (This)->lpVtbl -> put_AudioIn(This,Val) ) 

#define IAccess_get_AudioOut(This,pVal)	\
    ( (This)->lpVtbl -> get_AudioOut(This,pVal) ) 

#define IAccess_put_AudioOut(This,Val)	\
    ( (This)->lpVtbl -> put_AudioOut(This,Val) ) 

#define IAccess_get_CurrentUserHandle(This,pVal)	\
    ( (This)->lpVtbl -> get_CurrentUserHandle(This,pVal) ) 

#define IAccess_get_CurrentUserStatus(This,pVal)	\
    ( (This)->lpVtbl -> get_CurrentUserStatus(This,pVal) ) 

#define IAccess_put_CurrentUserStatus(This,newVal)	\
    ( (This)->lpVtbl -> put_CurrentUserStatus(This,newVal) ) 

#define IAccess_get_Mute(This,pVal)	\
    ( (This)->lpVtbl -> get_Mute(This,pVal) ) 

#define IAccess_put_Mute(This,newVal)	\
    ( (This)->lpVtbl -> put_Mute(This,newVal) ) 

#define IAccess_GetMessageA(This,MessageID,Message)	\
    ( (This)->lpVtbl -> GetMessageA(This,MessageID,Message) ) 

#define IAccess_SendMessageA(This,UserHandle,Text,Message)	\
    ( (This)->lpVtbl -> SendMessageA(This,UserHandle,Text,Message) ) 

#define IAccess_Connect(This)	\
    ( (This)->lpVtbl -> Connect(This) ) 

#define IAccess_GetActiveCallList(This,List)	\
    ( (This)->lpVtbl -> GetActiveCallList(This,List) ) 

#define IAccess_GetMissedCallList(This,List)	\
    ( (This)->lpVtbl -> GetMissedCallList(This,List) ) 

#define IAccess_GetMessageList(This,List)	\
    ( (This)->lpVtbl -> GetMessageList(This,List) ) 

#define IAccess_GetMissedMessageList(This,List)	\
    ( (This)->lpVtbl -> GetMissedMessageList(This,List) ) 

#define IAccess_SearchForMessages(This,UserHandle,List)	\
    ( (This)->lpVtbl -> SearchForMessages(This,UserHandle,List) ) 

#define IAccess_get_Version(This,pVal)	\
    ( (This)->lpVtbl -> get_Version(This,pVal) ) 

#define IAccess_get_Privilege(This,Name,pVal)	\
    ( (This)->lpVtbl -> get_Privilege(This,Name,pVal) ) 

#define IAccess_GetUser(This,Handle,User)	\
    ( (This)->lpVtbl -> GetUser(This,Handle,User) ) 

#define IAccess_get_SkypeExecutablePath(This,pVal)	\
    ( (This)->lpVtbl -> get_SkypeExecutablePath(This,pVal) ) 

#define IAccess_get_Protocol(This,pVal)	\
    ( (This)->lpVtbl -> get_Protocol(This,pVal) ) 

#define IAccess_put_Protocol(This,lngVal)	\
    ( (This)->lpVtbl -> put_Protocol(This,lngVal) ) 

#define IAccess_GetChatList(This,List)	\
    ( (This)->lpVtbl -> GetChatList(This,List) ) 

#define IAccess_GetChat(This,Name,Chat)	\
    ( (This)->lpVtbl -> GetChat(This,Name,Chat) ) 

#define IAccess_GetChatMessageList(This,List)	\
    ( (This)->lpVtbl -> GetChatMessageList(This,List) ) 

#define IAccess_GetMissedChatMessageList(This,List)	\
    ( (This)->lpVtbl -> GetMissedChatMessageList(This,List) ) 

#define IAccess_GetChatMessage(This,ID,Message)	\
    ( (This)->lpVtbl -> GetChatMessage(This,ID,Message) ) 

#define IAccess_CloseSkype(This)	\
    ( (This)->lpVtbl -> CloseSkype(This) ) 

#define IAccess_get_SkypeOutBalance(This,pVal)	\
    ( (This)->lpVtbl -> get_SkypeOutBalance(This,pVal) ) 

#define IAccess_get_SkypeOutCurrency(This,pVal)	\
    ( (This)->lpVtbl -> get_SkypeOutCurrency(This,pVal) ) 

#define IAccess_get_APIAttachmentStatus(This,APIStatus)	\
    ( (This)->lpVtbl -> get_APIAttachmentStatus(This,APIStatus) ) 

#define IAccess_PlaceConferenceCall(This,UserHandles,Call)	\
    ( (This)->lpVtbl -> PlaceConferenceCall(This,UserHandles,Call) ) 

#define IAccess_LaunchChat(This,UserHandles,Chat)	\
    ( (This)->lpVtbl -> LaunchChat(This,UserHandles,Chat) ) 

#define IAccess_OpenIMDialog(This,UserHandle,Message)	\
    ( (This)->lpVtbl -> OpenIMDialog(This,UserHandle,Message) ) 

#define IAccess_FocusSkypeWindow(This)	\
    ( (This)->lpVtbl -> FocusSkypeWindow(This) ) 

#define IAccess_LaunchAddAFriend(This,UserHandle)	\
    ( (This)->lpVtbl -> LaunchAddAFriend(This,UserHandle) ) 

#define IAccess_get_Conversion(This,pVal)	\
    ( (This)->lpVtbl -> get_Conversion(This,pVal) ) 

#define IAccess_MinimizeSkype(This)	\
    ( (This)->lpVtbl -> MinimizeSkype(This) ) 

#define IAccess_SearchForChatMessages(This,UserHandle,List)	\
    ( (This)->lpVtbl -> SearchForChatMessages(This,UserHandle,List) ) 

#define IAccess_GetActiveChatList(This,pVal)	\
    ( (This)->lpVtbl -> GetActiveChatList(This,pVal) ) 

#define IAccess_GetMissedChatList(This,pVal)	\
    ( (This)->lpVtbl -> GetMissedChatList(This,pVal) ) 

#define IAccess_GetRecentChatList(This,pVal)	\
    ( (This)->lpVtbl -> GetRecentChatList(This,pVal) ) 

#define IAccess_GetBookmarkedChatList(This,pVal)	\
    ( (This)->lpVtbl -> GetBookmarkedChatList(This,pVal) ) 

#define IAccess_GetVoiceMailList(This,pVal)	\
    ( (This)->lpVtbl -> GetVoiceMailList(This,pVal) ) 

#define IAccess_SendVoiceMail(This,UserHandle,pVal)	\
    ( (This)->lpVtbl -> SendVoiceMail(This,UserHandle,pVal) ) 

#define IAccess_GetUsersWaitingForAuthorization(This,pVal)	\
    ( (This)->lpVtbl -> GetUsersWaitingForAuthorization(This,pVal) ) 

#define IAccess_ClearChatHistory(This)	\
    ( (This)->lpVtbl -> ClearChatHistory(This) ) 

#define IAccess_ClearVoiceMailHistory(This)	\
    ( (This)->lpVtbl -> ClearVoiceMailHistory(This) ) 

#define IAccess_ClearCallHistory(This,CallType)	\
    ( (This)->lpVtbl -> ClearCallHistory(This,CallType) ) 

#define IAccess_get_Ringer(This,pVal)	\
    ( (This)->lpVtbl -> get_Ringer(This,pVal) ) 

#define IAccess_put_Ringer(This,Val)	\
    ( (This)->lpVtbl -> put_Ringer(This,Val) ) 

#define IAccess_ConnectAndWait(This,TimeoutInSeconds)	\
    ( (This)->lpVtbl -> ConnectAndWait(This,TimeoutInSeconds) ) 

#define IAccess_OpenFileTransferDialog(This,UserHandles,Folder)	\
    ( (This)->lpVtbl -> OpenFileTransferDialog(This,UserHandles,Folder) ) 

#define IAccess_SendBlockingCommand(This,Command,pVal)	\
    ( (This)->lpVtbl -> SendBlockingCommand(This,Command,pVal) ) 

#define IAccess_get_CommandTimeout(This,pVal)	\
    ( (This)->lpVtbl -> get_CommandTimeout(This,pVal) ) 

#define IAccess_put_CommandTimeout(This,Val)	\
    ( (This)->lpVtbl -> put_CommandTimeout(This,Val) ) 

#define IAccess_OpenApplication(This,ApplicationName,App)	\
    ( (This)->lpVtbl -> OpenApplication(This,ApplicationName,App) ) 

#define IAccess_get_Profile(This,pVal)	\
    ( (This)->lpVtbl -> get_Profile(This,pVal) ) 

#define IAccess_get_UI(This,pVal)	\
    ( (This)->lpVtbl -> get_UI(This,pVal) ) 

#define IAccess_get_VideoIn(This,pVal)	\
    ( (This)->lpVtbl -> get_VideoIn(This,pVal) ) 

#define IAccess_put_VideoIn(This,Val)	\
    ( (This)->lpVtbl -> put_VideoIn(This,Val) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAccess_INTERFACE_DEFINED__ */



#ifndef __SKYPEAPILib_LIBRARY_DEFINED__
#define __SKYPEAPILib_LIBRARY_DEFINED__

/* library SKYPEAPILib */
/* [helpfile][helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_SKYPEAPILib;

EXTERN_C const CLSID CLSID_Conversion;

#ifdef __cplusplus

class DECLSPEC_UUID("AFFEF374-D73A-4e17-8CAC-72ABDA7008BC")
Conversion;
#endif

EXTERN_C const CLSID CLSID_CallCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("1703F3AD-0353-45b2-8077-6833B8252B90")
CallCollection;
#endif

EXTERN_C const CLSID CLSID_Call;

#ifdef __cplusplus

class DECLSPEC_UUID("4EE25E18-0517-4bfd-A74B-D35BC8DF9F6D")
Call;
#endif

EXTERN_C const CLSID CLSID_UserCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("C2FA1611-6E4E-454d-8F21-BB149115F34C")
UserCollection;
#endif

EXTERN_C const CLSID CLSID_User;

#ifdef __cplusplus

class DECLSPEC_UUID("79ED0EAF-91EF-4eef-A197-0A9374837836")
User;
#endif

EXTERN_C const CLSID CLSID_App2App;

#ifdef __cplusplus

class DECLSPEC_UUID("C5155C83-C87A-4620-B63B-89584498DFA1")
App2App;
#endif

EXTERN_C const CLSID CLSID_App2AppStream;

#ifdef __cplusplus

class DECLSPEC_UUID("0CC09B21-449C-4d70-A304-5838500E108F")
App2AppStream;
#endif

EXTERN_C const CLSID CLSID_MessageCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("D3C0355D-B186-48bc-9F3A-61EC0537D0D4")
MessageCollection;
#endif

EXTERN_C const CLSID CLSID_Message;

#ifdef __cplusplus

class DECLSPEC_UUID("625229B6-53B2-4f05-9062-1E36C117B373")
Message;
#endif

EXTERN_C const CLSID CLSID_ChatMessageCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("6EBD5624-D4DE-441d-ABED-4CF8D487ECF9")
ChatMessageCollection;
#endif

EXTERN_C const CLSID CLSID_ChatMessage;

#ifdef __cplusplus

class DECLSPEC_UUID("A396E674-9241-463f-A71E-AD57BA579AD0")
ChatMessage;
#endif

EXTERN_C const CLSID CLSID_ChatCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("B8B7C427-DF53-4807-808A-1D7866A3A185")
ChatCollection;
#endif

EXTERN_C const CLSID CLSID_Chat;

#ifdef __cplusplus

class DECLSPEC_UUID("0FA5B4B9-DE42-4e96-9EEE-6515DB9DBA0E")
Chat;
#endif

EXTERN_C const CLSID CLSID_VoiceMailCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("6D49D4A5-22A1-471e-A670-88BDFC2A533A")
VoiceMailCollection;
#endif

EXTERN_C const CLSID CLSID_VoiceMail;

#ifdef __cplusplus

class DECLSPEC_UUID("8B7ED723-0B44-4d6c-8E6B-1F1E424FD549")
VoiceMail;
#endif

EXTERN_C const CLSID CLSID_UI;

#ifdef __cplusplus

class DECLSPEC_UUID("D9770D9B-BB30-4a82-BC01-83B110408D5C")
UI;
#endif

EXTERN_C const CLSID CLSID_Profile;

#ifdef __cplusplus

class DECLSPEC_UUID("154ED84A-F2A2-407a-BB81-BCFEAAAD463D")
Profile;
#endif

#ifndef ___IAccessEvents_DISPINTERFACE_DEFINED__
#define ___IAccessEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IAccessEvents */
/* [helpstring][helpcontext][uuid] */ 


EXTERN_C const IID DIID__IAccessEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F2858E1B-90D5-4457-A096-2B19898ADAB2")
    _IAccessEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IAccessEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IAccessEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IAccessEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IAccessEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IAccessEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IAccessEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IAccessEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IAccessEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IAccessEventsVtbl;

    interface _IAccessEvents
    {
        CONST_VTBL struct _IAccessEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IAccessEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IAccessEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IAccessEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IAccessEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IAccessEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IAccessEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IAccessEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IAccessEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Access;

#ifdef __cplusplus

class DECLSPEC_UUID("06406675-E635-4fe4-8B2C-82C5C0BA6F40")
Access;
#endif
#endif /* __SKYPEAPILib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


