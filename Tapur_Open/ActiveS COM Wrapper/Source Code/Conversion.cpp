/*
Copyright (c) 2005, Jason Terando
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the parties distributing the library nor the names of its contributors may be
      used to endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
// Conversion.cpp : Implementation of Conversion

#include "stdafx.h"
#include "DateTime.h"
#include "Conversion.h"

SkypeCallProgress TextToSkypeCallProgress(LPCWSTR Text)
{
	if(Text == NULL)
		return prgUnknown;
	else if(_wcsicmp(Text, L"UNPLACED") == 0)
		return prgUnplaced;
	else if(_wcsicmp(Text, L"ROUTING") == 0)
		return prgRouting;
	else if(_wcsicmp(Text, L"EARLYMEDIA") == 0)
		return prgEarlyMedia;
	else if(_wcsicmp(Text, L"FAILED") == 0)
		return prgFailed;
	else if(_wcsicmp(Text, L"RINGING") == 0)
		return prgRinging;
	else if(_wcsicmp(Text, L"INPROGRESS") == 0)
		return prgInProgress;
	else if(_wcsicmp(Text, L"ONHOLD") == 0)
		return prgOnHold;
	else if(_wcsicmp(Text, L"FINISHED") == 0)
		return prgFinished;
	else if(_wcsicmp(Text, L"MISSED") == 0)
		return prgMissed;
	else if(_wcsicmp(Text, L"REFUSED") == 0) 
		return prgRefused;
	else if(_wcsicmp(Text, L"BUSY") == 0) 
		return prgBusy;
	else if(_wcsicmp(Text, L"CANCELLED") == 0) 
		return prgCancelled;
	else if(_wcsicmp(Text, L"REMOTEHOLD") == 0) 
		return prgRemoteHold;
	else if(_wcsicmp(Text, L"LOCALHOLD") == 0) 
		return prgLocalHold;
	else if(_wcsicmp(Text, L"VM_BUFFERING_GREETING") == 0) 
		return prgBufferingGreeting;
	else if(_wcsicmp(Text, L"VM_PLAYING_GREETING") == 0) 
		return prgPlayingGreeting;
	else if(_wcsicmp(Text, L"VM_RECORDING") == 0) 
		return prgRecordingVoiceMail;
	else if(_wcsicmp(Text, L"VM_UPLOADING") == 0) 
		return prgUploadingVoiceMail;
	else if(_wcsicmp(Text, L"VM_SENT") == 0) 
		return prgSentVoiceMail;
	else if(_wcsicmp(Text, L"VM_CANCELLED") == 0) 
		return prgCancelledVoiceMail;
	else if(_wcsicmp(Text, L"VM_FAILED") == 0) 
		return prgFailedVoiceMail;
	else
		return prgUnknown;
}

SkypeCallType TextToSkypeCallType(LPCWSTR Text)
{
	if(Text == NULL)
		return ctypUnknown;
	else if(_wcsicmp(Text, L"INCOMING_PSTN") == 0)
		return ctypIncomingPSTN;
	else if(_wcsicmp(Text, L"OUTGOING_PSTN") == 0)
		return ctypOutgoingPSTN;
	else if(_wcsicmp(Text, L"INCOMING_P2P") == 0)
		return ctypIncomingP2P;
	else if(_wcsicmp(Text, L"OUTGOING_P2P") == 0)
		return ctypOutgoingP2P;
	else
		return ctypUnknown;
}

SkypeMessageStatus TextToSkypeMesasgeStatus(LPCWSTR Text)
{
	if(Text == NULL)
		return msgUnknown;
	else if(_wcsicmp(Text, L"SENDING") == 0)
		return msgSending;
	else if(_wcsicmp(Text, L"SENT") == 0)
		return msgSent;
	else if(_wcsicmp(Text, L"FAILED") == 0)
		return msgFailed;
	else if(_wcsicmp(Text, L"RECEIVED") == 0)
		return msgReceived;
	else if(_wcsicmp(Text, L"READ") == 0)
		return msgRead;
	else if(_wcsicmp(Text, L"IGNORED") == 0)
		return msgIgnored;
	else if(_wcsicmp(Text, L"QUEUED") == 0)
		return msgQueued;
	else
		return msgUnknown;
}

SkypeMessageType TextToSkypeMessageType(LPCWSTR Text)
{
	if(Text == NULL)
		return mtypUnknown;
	else if(_wcsicmp(Text, L"AUTHREQUEST") == 0)
		return mtypAuthRequest;
	else if(_wcsicmp(Text, L"TEXT") == 0)
		return mtypText;
	else if(_wcsicmp(Text, L"CONTACTS") == 0)
		return mtypContacts;
	else
		return mtypUnknown;
}

SkypeUserSex TextToSkypeUserSex(LPCWSTR Text)
{
	if(Text == NULL)
		return usexUnknown;
	else if(_wcsicmp(Text, L"UNKNOWN") == 0)
		return usexNotSpecified;
	else if(_wcsicmp(Text, L"MALE") == 0)
		return usexMale;
	else if(_wcsicmp(Text, L"FEMALE") == 0)
		return usexFemale;
	else
		return usexUnknown;
}

SkypeBuddyStatus TextToSkypeBuddyStatus(LPCWSTR Text)
{
	if(Text == NULL)
		return budUnknown;
	else if(_wcsicmp(Text, L"0") == 0)
		return budNever;
	else if(_wcsicmp(Text, L"1") == 0)
		return budDeleted;
	else if(_wcsicmp(Text, L"2") == 0)
		return budPending;
	else if(_wcsicmp(Text, L"3") == 0)
		return budFriend;
	else
		return budUnknown;
};

SkypeOnlineStatus TextToSkypeOnlineStatus(LPCWSTR Text)
{
	if(Text == NULL)
		return olsUnknown;
	else if(_wcsicmp(Text, L"OFFLINE") == 0)
		return olsOffline;
	else if(_wcsicmp(Text, L"ONLINE") == 0)
		return olsOnline;
	else if(_wcsicmp(Text, L"AWAY") == 0)
		return olsAway;
	else if(_wcsicmp(Text, L"NA") == 0)
		return olsNotAvail;
	else if(_wcsicmp(Text, L"DND") == 0)
		return olsDoNotDisturb;
	else if(_wcsicmp(Text, L"INVISIBLE") == 0)
		return olsInvisible;
	else if(_wcsicmp(Text, L"SKYPEME") == 0)
		return olsSkypeMe;
	else if(_wcsicmp(Text, L"LOGGEDOUT") == 0)
		return olsLoggedOut;
	else
		return olsUnknown;
};

SkypeConnectionStatus TextToSkypeConnectionStatus(LPCWSTR Text)
{
	if(Text == NULL)
		return conUnknown;
	else if(_wcsicmp(Text, L"OFFLINE") == 0)
		return conOffline;
	else if(_wcsicmp(Text, L"CONNECTING") == 0)
		return conConnecting;
	else if(_wcsicmp(Text, L"PAUSING") == 0)
		return conPausing;
	else if(_wcsicmp(Text, L"ONLINE") == 0)
		return conOnline;
	else if(_wcsicmp(Text, L"LOGGEDOUT") == 0)
		return conLoggedOut;
	else
		return conUnknown;
};

SkypeChatStatus TextToSkypeChatStatus(LPCWSTR Text)
{
	if(Text == NULL)
		return chatUnknown;
	else if(_wcsicmp(Text, L"LEGACY_DIALOG") == 0)
		return chatLegacyDialog;
	else if(_wcsicmp(Text, L"DIALOG") == 0)
		return chatDialog;
	else if(_wcsicmp(Text, L"MULTI_NEEDACCEPT") == 0)
		return chatMultiNeedAccept;
	else if(_wcsicmp(Text, L"MULTI_SUBSCRIBED") == 0)
		return chatMultiSubscribed;
	else
		return chatUnknown;
};


SkypeChatMessageStatus TextToSkypeChatMessageStatus(LPCWSTR Text)
{
	if(Text == NULL)
		return chatMsgUnknown;
	else if (_wcsicmp(Text, L"SENDING") == 0)
		return chatMsgSending;
	else if (_wcsicmp(Text, L"SENT") == 0)
		return chatMsgSent;
	else if (_wcsicmp(Text, L"RECEIVED") == 0)
		return chatMsgReceived;
	else if (_wcsicmp(Text, L"READ") == 0)
		return chatMsgRead;
	else
		return chatMsgUnknown;
}

SkypeChatMessageLeftReason TextToSkypeChatMessageLeftReason(LPCWSTR Text)
{
	if(Text == NULL)
		return chatMsgLeftUnknown;
	else if (_wcsicmp(Text, L"USER_NOT_FOUND") == 0)
		return chatMsgLeftUserNotFound;
	else if (_wcsicmp(Text, L"USER_INCAPABLE") == 0)
		return chatMsgLeftUserIncapable;
	else if (_wcsicmp(Text, L"ADDER_MUST_BE_FRIEND") == 0)
		return chatMsgLeftAdderMustBeFriend;
	else if (_wcsicmp(Text, L"ADDER_MUST_BE_AUTHORIZED") == 0)
		return chatMsgLeftAdderMustBeAuthorized;
	else if (_wcsicmp(Text, L"DECLINE_ADD") == 0)
		return chatMsgLeftDeclineAdd;
	else if (_wcsicmp(Text, L"") == 0)
		return chatMsgLeftUnsubscribe;
	else
		return chatMsgLeftUnknown;
}

SkypeCurrencyType TextToSkypeCurrencyType(LPCWSTR Text)
{
	if(Text == NULL) {
		return currUnknown;
	} else if (_wcsicmp(Text, L"EUR") == 0) {
		return currEuroCents;
	} else {
		return currUnknown;
	}
}

SkypeChatMessageType TextToSkypeChatMessageType(LPCWSTR Text)
{
	if(Text == NULL) {
		return chatMsgTypeUnknown;
	} else if (_wcsicmp(Text, L"SETTOPIC") == 0) {
		return chatMsgTypeSetTopic;
	} else if (_wcsicmp(Text, L"SAID") == 0) {
		return chatMsgTypeSaid;
	} else if (_wcsicmp(Text, L"ADDEDMEMBERS") == 0) {
		return chatMsgTypeAddedMembers;
	} else if (_wcsicmp(Text, L"SAWMEMBERS") == 0) {
		return chatMsgTypeSawMembers;
	} else if (_wcsicmp(Text, L"CREATEDCHATWITH") == 0) {
		return chatMsgTypeCreatedChatWith;
	} else if (_wcsicmp(Text, L"LEFT") == 0) {
		return chatMsgTypeLeft;
	} else {
		return chatMsgTypeUnknown;
	}
}

SkypeVoiceMailType TextToSkypeVoiceMailType(LPCWSTR Text)
{
	if(Text == NULL) {
		return vmTypeUnknown;
	} else if (_wcsicmp(Text, L"INCOMING") == 0) {
		return vmTypeIncoming;
	} else if (_wcsicmp(Text, L"DEFAULT_GREETING") == 0) {
		return vmTypeDefaultGreeting;
	} else if (_wcsicmp(Text, L"CUSTOM_GREETING") == 0) {
		return vmTypeCustomGreeting;
	} else if (_wcsicmp(Text, L"OUTGOING") == 0) {
		return vmTypeOutgoing;
	} else {
		return vmTypeUnknown;
	}
}

SkypeVoiceMailStatus TextToSkypeVoiceMailStatus(LPCWSTR Text)
{
	if(Text == NULL) {
		return vmStatusUnknown;
	} else if (_wcsicmp(Text, L"NOWDOWNLOADED") == 0) {
		return vmStatusDownloaded;
	} else if (_wcsicmp(Text, L"DOWNLOADING") == 0) {
		return vmStatusDownloading;
	} else if (_wcsicmp(Text, L"UNPLAYED") == 0) {
		return vmStatusUnplayed;
	} else if (_wcsicmp(Text, L"BUFFERING") == 0) {
		return vmStatusBuffering;
	} else if (_wcsicmp(Text, L"PLAYING") == 0) {
		return vmStatusPlaying;
	} else if (_wcsicmp(Text, L"PLAYED") == 0) {
		return vmStatusPlayed;
	} else if (_wcsicmp(Text, L"BLANK") == 0) {
		return vmStatusBlank;
	} else if (_wcsicmp(Text, L"RECORDING") == 0) {
		return vmStatusRecording;
	} else if (_wcsicmp(Text, L"RECORDED") == 0) {
		return vmStatusRecorded;
	} else if (_wcsicmp(Text, L"UPLOADING") == 0) {
		return vmStatusUploading;
	} else if (_wcsicmp(Text, L"UPLOADED") == 0) {
		return vmStatusUploaded;
	} else if (_wcsicmp(Text, L"DELETING") == 0) {
		return vmStatusDeleting;
	} else if (_wcsicmp(Text, L"FAILED") == 0) {
		return vmStatusFailed;
	} else {
		return vmStatusUnknown;
	}
}

SkypeVoiceMailFailure TextToSkypeVoiceMailFailure(LPCWSTR Text)
{
	if(Text == NULL) {
		return vmFailureUnknown;
	} else if (_wcsicmp(Text, L"MISC_ERROR") == 0) {
		return vmFailureMisc;
	} else if (_wcsicmp(Text, L"CONNECT_ERROR") == 0) {
		return vmFailureConnect;
	} else if (_wcsicmp(Text, L"NO_VOICEMAIL_PRIVILEGE") == 0) {
		return vmFailureNoPrivilege;
	} else if (_wcsicmp(Text, L"NO_SUCH_VOICEMAIL") == 0) {
		return vmFailureNoSuchVoiceMail;
	} else if (_wcsicmp(Text, L"FILE_READ_ERROR") == 0) {
		return vmFailureReadError;
	} else if (_wcsicmp(Text, L"FILE_WRITE_ERROR") == 0) {
		return vmFailureWriteError;
	} else if (_wcsicmp(Text, L"RECORDING_ERROR") == 0) {
		return vmFailureRecording;
	} else if (_wcsicmp(Text, L"PLAYBACK_ERROR") == 0) {
		return vmFailurePlayback;
	} else {
		return vmFailureUnknown;
	}
}

SkypeNoAnswerAction TextToSkypeNoAnswerAction(LPCWSTR Text)
{
	if(Text == NULL) {
		return noAnsUnknown;
	} else if (_wcsicmp(Text, L"REJECT") == 0) {
		return noAnsReject;
	} else if (_wcsicmp(Text, L"FORWARD") == 0) {
		return noAnsForward;
	} else if (_wcsicmp(Text, L"VOICEMAIL") == 0) {
		return noAnsVoicemail;
	} else {
		return noAnsUnknown;
	}
}

BSTR ErrorCodeToBSTR(LONG ErrorCode)
{
	switch(ErrorCode) {
	case 1:
		return L"General Syntax Error";
	case 2:
		return L"Unknown Command";
	case 3:
		return L"Search: Unknown Search Target";
	case 4:
		return L"Empty Target Not Allowed";
	case 5:
		return L"Search CALLS: Invalid Target";
	case 6:
		return L"Target Not Allowed with MIXEDMESSAGES";
	case 7:
		return L"GET: Invalid WHAT";
	case 8:
		return L"Invalid User Handle";
	case 9:
		return L"Unknown User";
	case 10:
		return L"Invalid Property";
	case 11:
		return L"Invalid Call ID";
	case 12:
		return L"Unknown Call";
	case 13:
		return L"Invalid PROP";
	case 14:
		return L"Invalid Message ID";
	case 15:
		return L"Unknown Message";
	case 16:
		return L"GET: Invalid Property";
	case 17:
		return L"Not In Use";
	case 18:
		return L"SET: Invalid Property";
	case 19:
		return L"Invalid Call ID";
	case 20:
		return L"Unknown Call";
	case 21:
		return L"Unknown/Disallowed Call Property";
	case 22:
		return L"Cannot Hold This Call At This Moment";
	case 23:
		return L"Cannot Resume This Call At This Moment";
	case 24:
		return L"Cannot Hangup Inactive Call";
	case 25:
		return L"SET: Invalid Propety";
	case 26:
		return L"Invalid User Handle";
	case 27:
		return L"Invalid Version Number";
	case 28:
		return L"Invalid User Status";
	case 29:
		return L"Target Not Allowed With MISSEDCALLS";
	case 30:
		return L"Invalid Message ID";
	case 31:
		return L"Unknown Message ID";
	case 32:
		return L"Invalid Property";
	case 33:
		return L"Invalid Parameter to SET MUTE";
	case 34:
		return L"Invalid User Handle to CALL";
	case 35:
		return L"Not Connected";
	case 36:
		return L"Not Online";
	case 37:
		return L"Not Connected";
	case 38:
		return L"Not Online";
	case 39:
		return L"User Blocked";
	case 40:
		return L"Unknown Privilege";
	case 41:
		return L"Call Not Active";
	case 42:
		return L"Invalid DTMF Code";
	case 66:
		return L"Not Connected";
	case 67:
		return L"Target Not Allowed With SEARCH FRIENDS";
	case 69:
		return L"Invalid Open WHAT";
	case 70:
		return L"Invalid Handle";
	case 91:
		return L"Cannot Call An Emergency Number";
	case 92:
		return L"The Number Called Is Not A Valid POTS Number";
	case 93:
		return L"Invalid Skype Name";
	case 94:
		return L"Cannot Call Yourself";
	case 95:
		return L"Destination user is blocked by caller right after call initialization";
	case 96:
		return L"An outgoing call exists in ROUTING/RINGING/EARLYMEDIA state";
	case 97:
		return L"Internal Error";
	case 98:
		return L"Internal Error";
	case 99:
		return L"Internal Error";
	case 100:
		return L"Internal Error";
	case 101:
		return L"A call to the destination user is already ongoing";
	case 103:
		return L"Internal Error";
	case 104:
		return L"Internal Error";
	case 9901:
		return L"Internal Error";
	default:
		return L"Unknown Error";
	}
}

DATE UnixTimestampToDATE(time_t t)
{
	CDateTime dt(t);
	return dt.m_dt;
}

// Conversion
STDMETHODIMP Conversion::SkypeCallProgressAsText(SkypeCallProgress Progress, BSTR *Text)
{
	switch(Progress) {
		case prgUnplaced:
			*Text = SysAllocString(L"Call was never placed");
			break;
		case prgRouting:
			*Text = SysAllocString(L"Call Is Currently Being Routed");
			break;
		case prgEarlyMedia:
			*Text = SysAllocString(L"\"Early Media\" Is Being Played");
			break;
		case prgFailed:
			*Text = SysAllocString(L"Call Failed");
			break;
		case prgRinging:
			*Text = SysAllocString(L"Ringing");
			break;
		case prgInProgress:
			*Text = SysAllocString(L"Call In Progress");
			break;
		case prgOnHold:
			*Text = SysAllocString(L"Call Is On Hold");
			break;
		case prgFinished:
			*Text = SysAllocString(L"Call Is Finished");
			break;
		case prgRefused:
			*Text = SysAllocString(L"Call Was Refused");
			break;
		case prgMissed:
			*Text = SysAllocString(L"Call Was Missed");
			break;
		case prgBusy:
			*Text = SysAllocString(L"Destination is Busy");
			break;
		case prgCancelled:
			*Text = SysAllocString(L"Call was cancelled");
			break;
		case prgRemoteHold:
			*Text = SysAllocString(L"Call was placed on hold remotely");
			break;
		case prgLocalHold:
			*Text = SysAllocString(L"Call was placed on hold locally");
			break;
		case prgBufferingGreeting:
			*Text = SysAllocString(L"Buffering VoiceMail Greeting");
			break;
		case prgPlayingGreeting:
			*Text = SysAllocString(L"Playing VoiceMail Greeting");
			break;
		case prgRecordingVoiceMail:
			*Text = SysAllocString(L"Recording VoiceMail");
			break;
		case prgUploadingVoiceMail:
			*Text = SysAllocString(L"Uploading VoiceMail");
			break;
		case prgSentVoiceMail:
			*Text = SysAllocString(L"Sent VoiceMail");
			break;
		case prgCancelledVoiceMail:
			*Text = SysAllocString(L"Cancelled VoiceMail");
			break;
		case prgFailedVoiceMail:
			*Text = SysAllocString(L"Failed VoiceMail");
			break;
		default: // prgUnknown;
			*Text = SysAllocString(L"");
			break;
	}
	return S_OK;
}


STDMETHODIMP Conversion::SkypeCallTypeAsText(SkypeCallType CallType, BSTR * Text)
{
	switch(CallType) {
		case ctypIncomingPSTN:
			*Text = SysAllocString(L"Incoming Telephone Call");
			break;
		case ctypOutgoingPSTN:
			*Text = SysAllocString(L"Outgoing Telephone Call");
			break;
		case ctypIncomingP2P:
			*Text = SysAllocString(L"Incoming Peer-to-Peer");
			break;
		case ctypOutgoingP2P:
			*Text = SysAllocString(L"Outgoing Peer-to-Peer");
			break;
		default:
			*Text = SysAllocString(L"");
	}

	return S_OK;
}

STDMETHODIMP Conversion::SkypeMessageStatusAsText(SkypeMessageStatus Status, BSTR *Text)
{
	switch(Status) {
		case msgSending:
			*Text = SysAllocString(L"Message Is Begin Sent");
			break;
		case msgSent:
			*Text = SysAllocString(L"Message Was Sent");
			break;
		case msgFailed:
			*Text = SysAllocString(L"Message Sending Failed");
			break;
		case msgReceived:
			*Text = SysAllocString(L"Message Was Received");
			break;
		case msgRead:
			*Text = SysAllocString(L"Message Was Read");
			break;
		case msgIgnored:
			*Text = SysAllocString(L"Message Was Ignored");
			break;
		case msgQueued:
			*Text = SysAllocString(L"Message Was Queued");
			break;
		default:
			*Text = SysAllocString(L"");
	}

	return S_OK;
}

STDMETHODIMP Conversion::SkypeMessageTypeAsText(SkypeMessageType Type, BSTR *Text)
{
	switch(Type) {
		case mtypAuthRequest:
			*Text = SysAllocString(L"Authorization Request");
			break;
		case mtypText:
			*Text = SysAllocString(L"Text");
			break;
		case mtypContacts:
			*Text = SysAllocString(L"Contact Information");
			break;
		default:
			*Text = SysAllocString(L"");
			break;
	}

	return S_OK;
}

STDMETHODIMP Conversion::SkypeUserSexAsText(SkypeUserSex Sex, BSTR *Text)
{
	switch(Sex) {
		case usexNotSpecified:
			*Text = SysAllocString(L"Not Specified");
			break;
		case usexMale:
			*Text = SysAllocString(L"Male");
			break;
		case usexFemale:
			*Text = SysAllocString(L"Female");
			break;
		default:
			*Text = SysAllocString(L"");
			break;
	}

	return S_OK;
}

STDMETHODIMP Conversion::SkypeBuddyStatusAsText(SkypeBuddyStatus Status, BSTR *Text)
{
	switch(Status) {
		case budNever:
			*Text = SysAllocString(L"Never Been In Friendlist");
			break;
		case budDeleted:
			*Text = SysAllocString(L"Deleted From Friendlist");
			break;
		case budPending:
			*Text = SysAllocString(L"Pending Authorization");
			break;
		case budFriend:
			*Text = SysAllocString(L"Friend");
			break;
		default:
			*Text = SysAllocString(L"");
	}

	return S_OK;
}

STDMETHODIMP Conversion::SkypeOnlineStatusAsText(SkypeOnlineStatus Status, BSTR *Text)
{
	switch(Status) {
		case olsOffline:
			*Text = SysAllocString(L"Offline");
			break;
		case olsOnline:
			*Text = SysAllocString(L"Online");
			break;
		case olsAway:
			*Text = SysAllocString(L"Away");
			break;
		case olsNotAvail:
			*Text = SysAllocString(L"Not Available");
			break;
		case olsDoNotDisturb:
			*Text = SysAllocString(L"Do Not Disturb");
			break;
		case olsInvisible:
			*Text = SysAllocString(L"Invisible");
			break;
		case olsSkypeMe:
			*Text = SysAllocString(L"Skype Me");
			break;
		case olsLoggedOut:
			*Text = SysAllocString(L"Logged Out");
			break;
		default:
			*Text = SysAllocString(L"");
			break;
	}

	return S_OK;
}


STDMETHODIMP Conversion::SkypeConnectionStatusAsText(SkypeConnectionStatus Status, BSTR *Text)
{
	switch(Status) {
		case conOffline:
			*Text = SysAllocString(L"Offline");
			break;
		case conOnline:
			*Text = SysAllocString(L"Online");
			break;
		case conPausing:
			*Text = SysAllocString(L"Pausing");
			break;
		case conConnecting:
			*Text = SysAllocString(L"Connecting");
			break;
		case conLoggedOut:
			*Text = SysAllocString(L"Logged Out");
			break;
		default:
			*Text = SysAllocString(L"");
			break;
	}

	return S_OK;
}


STDMETHODIMP Conversion::SkypeAPIAttachmentStatusAsText(SkypeAPIAttachmentStatus Status, BSTR *Text)
{
	switch(Status) {
		case apiAttachSuccess:
			*Text = SysAllocString(L"API Attachment Successful");
			break;
		case apiAttachPendingAuth:
			*Text = SysAllocString(L"API Attachment Pending Authorization");
			break;
		case apiAttachRefused:
			*Text = SysAllocString(L"API Attachment Refused");
			break;
		case apiAttachNotAvailable:
			*Text = SysAllocString(L"API Attachment Not Available");
			break;
		case apiAttachAPIAvail:
			*Text = SysAllocString(L"API Attachment Available");
			break;
		default:
			*Text = SysAllocString(L"");
			break;
	}
	return S_OK;
}

STDMETHODIMP Conversion::SkypeChatStatusAsText(SkypeChatStatus ChatStatus, BSTR * Text)
{
	switch(ChatStatus) {
		case chatLegacyDialog:
			*Text = SysAllocString(L"Legacy Dialog");
			break;
		case chatDialog:
			*Text = SysAllocString(L"Dialog");
			break;
		case chatMultiNeedAccept:
			*Text = SysAllocString(L"Multi Need Accept");
			break;
		case chatMultiSubscribed:
			*Text = SysAllocString(L"Multi Subscribed");
			break;
		default:
			*Text = SysAllocString(L"");
			break;
	}

	return S_OK;
}

STDMETHODIMP Conversion::SkypeChatMessageStatusAsText(SkypeChatMessageStatus Status, BSTR *Text)
{
	switch(Status) {
		case chatMsgSending:
			*Text = SysAllocString(L"Sending");
			break;
		case chatMsgSent:
			*Text = SysAllocString(L"Sent");
			break;
		case chatMsgReceived:
			*Text = SysAllocString(L"Received");
			break;
		case chatMsgRead:
			*Text = SysAllocString(L"Read");
			break;
		default:
			*Text = SysAllocString(L"");
			break;
	}

	return S_OK;
}

STDMETHODIMP Conversion::SkypeChatMessageLeftReasonAsText(SkypeChatMessageLeftReason Reason, BSTR *Text)
{
	switch(Reason) {
		case chatMsgLeftUserNotFound:
			*Text = SysAllocString(L"User Not Found");
			break;
		case chatMsgLeftUserIncapable:
			*Text = SysAllocString(L"User Incapable");
			break;
		case chatMsgLeftAdderMustBeFriend:
			*Text = SysAllocString(L"Adder Must Be Friend");
			break;
		case chatMsgLeftAdderMustBeAuthorized:
			*Text = SysAllocString(L"Adder Must Be Authorized");
			break;
		case chatMsgLeftDeclineAdd:
			*Text = SysAllocString(L"Declined to Add");
			break;
		case chatMsgLeftUnsubscribe:
			*Text = SysAllocString(L"Unsubscribe");
			break;
		default:
			*Text = SysAllocString(L"");
			break;
	}

	return S_OK;
}

STDMETHODIMP Conversion::SkypeCurrencyTypeAsText(SkypeCurrencyType Currency, BSTR *Text)
{
	switch(Currency) {
		case currEuroCents:
			*Text = SysAllocString(L"Euro Cents");
			break;
		default:
			*Text = SysAllocString(L"SkypeOut Unavailable");
			break;
	}

	return S_OK;
}

STDMETHODIMP Conversion::SkypeChatMessageTypeAsText(SkypeChatMessageType MessageType, BSTR *Text)
{
	switch(MessageType) {
		case chatMsgTypeSetTopic:
			*Text = SysAllocString(L"Change in Chat Topic");
			break;
		case chatMsgTypeSaid:
			*Text = SysAllocString(L"Instant Message");
			break;
		case chatMsgTypeAddedMembers:
			*Text = SysAllocString(L"Invite to Chat");
			break;
		case chatMsgTypeSawMembers:
			*Text = SysAllocString(L"Chat Participant Has Seen Other Members");
			break;
		case chatMsgTypeCreatedChatWith:
			*Text = SysAllocString(L"Chat to Multiple People Created");
			break;
		case chatMsgTypeLeft:
			*Text = SysAllocString(L"Somebody Left Chat");
			break;
		default:
			*Text = SysAllocString(L"Unknown");
			break;
	}
	
	return S_OK;
}

STDMETHODIMP Conversion::SkypeVoiceMailStatusAsText(SkypeVoiceMailStatus VoiceMailStatus, BSTR *Text)
{
	switch(VoiceMailStatus) {
		case vmStatusDownloading:
			*Text = SysAllocString(L"Downloading");
			break;
		case vmStatusDownloaded:
			*Text = SysAllocString(L"Downloaded");
			break;
		case vmStatusUnplayed:
			*Text = SysAllocString(L"Unplayed");
			break;
		case vmStatusBuffering:
			*Text = SysAllocString(L"Buffering");
			break;
		case vmStatusPlaying:
			*Text = SysAllocString(L"Playing");
			break;
		case vmStatusPlayed:
			*Text = SysAllocString(L"Played");
			break;
		case vmStatusBlank:
			*Text = SysAllocString(L"Blank");
			break;
		case vmStatusRecording:
			*Text = SysAllocString(L"Recording");
			break;
		case vmStatusRecorded:
			*Text = SysAllocString(L"Recorded");
			break;
		case vmStatusUploading:
			*Text = SysAllocString(L"Uploading");
			break;
		case vmStatusUploaded:
			*Text = SysAllocString(L"Uploaded");
			break;
		case vmStatusDeleting:
			*Text = SysAllocString(L"Deleting");
			break;
		case vmStatusFailed:
			*Text = SysAllocString(L"Failed");
			break;
		// case vmStatusUnknown:
		default:
			*Text = SysAllocString(L"Unknown");
			break;
	}
	return S_OK;
}

STDMETHODIMP Conversion::SkypeVoiceMailTypeAsText(SkypeVoiceMailType VoiceMailType, BSTR *Text)
{
	switch(VoiceMailType) {
		case vmTypeIncoming:
			*Text = SysAllocString(L"Incoming");
			break;
		case vmTypeDefaultGreeting:
			*Text = SysAllocString(L"Default Greeting");
			break;
		case vmTypeCustomGreeting:
			*Text = SysAllocString(L"Custom Greeting");
			break;
		case vmTypeOutgoing:
			*Text = SysAllocString(L"Outgoing");
			break;
		// case vmTypeUnknown:
		default:
			*Text = SysAllocString(L"Unknown");
			break;
	}

	return S_OK;
}

STDMETHODIMP Conversion::SkypeVoiceMailFailureAsText(SkypeVoiceMailFailure FailureReason, BSTR *Text)
{
	switch(FailureReason) {
		case vmFailureMisc:
			*Text = SysAllocString(L"Miscellaneous Error");
			break;
		case vmFailureConnect:
			*Text = SysAllocString(L"Connection Error");
			break;
		case vmFailureNoPrivilege:
			*Text = SysAllocString(L"No VoiceMail Privilege");
			break;
		case vmFailureNoSuchVoiceMail:
			*Text = SysAllocString(L"No Such VoiceMail");
			break;
		case vmFailureReadError:
			*Text = SysAllocString(L"File Read Error");
			break;
		case vmFailureWriteError:
			*Text = SysAllocString(L"File Write Error");
			break;
		case vmFailureRecording:
			*Text = SysAllocString(L"Recording Error");
			break;
		case vmFailurePlayback:
			*Text = SysAllocString(L"Playback Error");
			break;
		// case vmFailureUnknown:
		default:
			*Text = SysAllocString(L"Unknown");
			break;
	}

	return S_OK;
}

STDMETHODIMP Conversion::SkypeNoAnswerActionAsText(SkypeNoAnswerAction Action, BSTR *Text)
{
	switch(Action) {
		case noAnsReject:
			*Text = SysAllocString(L"Reject");
			break;
		case noAnsForward:
			*Text = SysAllocString(L"Forward");
			break;
		case noAnsVoicemail:
			*Text = SysAllocString(L"Voicemail");
			break;
		default:
			*Text = SysAllocString(L"Unknown");
	}

	return S_OK;
}
