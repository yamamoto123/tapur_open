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

2010 Yamamoto:change enum definition
*/
#pragma once

[import(".\\ForwardDefs.idl")];

[
	export,
	v1_enum,
	uuid("65BB3F87-9AC8-456e-BB0B-3C8034FD1371"),
	helpcontext(0101)
]
enum SkypeAPIAttachmentStatus {
	apiAttachUnknown = -1,
	apiAttachSuccess = 0,
	apiAttachPendingAuth = 1,
	apiAttachRefused = 2,
	apiAttachNotAvailable = 3,
	apiAttachAPIAvail = 0x8001
};

[
	export,
	v1_enum,
	uuid("8FCBDB89-66FF-4638-ABB2-CD9B41CE70D7"),
	helpcontext(0102)
]
enum SkypeConnectionStatus {
	conUnknown = -1,
	conOffline = 0,
	conConnecting = 1,
	conPausing = 2,
	conOnline = 3,
	conLoggedOut = 4
};

[
	export,
	v1_enum,
	uuid("A92E0B9F-DAF6-4092-A772-E920FEDDCCE9"),
	helpcontext(0103)
]
enum SkypeCallProgress {
	prgUnknown = -1,
	prgUnplaced = 0,
	prgRouting = 1,
	prgEarlyMedia = 2,
	prgFailed = 3,
	prgRinging = 4,
	prgInProgress = 5,
	prgOnHold = 6,
	prgFinished = 7,
	prgMissed = 8,
	prgRefused = 9,
	prgBusy = 10,
	prgCancelled = 11,
	prgRemoteHold = 12,
	prgLocalHold = 13,
	prgBufferingGreeting = 14,
	prgPlayingGreeting = 15,
	prgRecordingVoiceMail = 16,
	prgUploadingVoiceMail = 17,
	prgSentVoiceMail = 18,
	prgCancelledVoiceMail = 19,
	prgFailedVoiceMail = 20
};

[
	export,
	v1_enum,
	uuid("3859796A-5B13-4400-80B2-254516AEC067"),
	helpcontext(0104)
]
enum SkypeCallType {
	ctypUnknown = -1,
	ctypIncomingPSTN = 0,
	ctypOutgoingPSTN = 1,
	ctypIncomingP2P = 2,
	ctypOutgoingP2P = 3
};

[
	export,
	v1_enum,
	uuid("33C4110A-02D8-4092-AC69-B7B258786762"),
	helpcontext(0105)
]
enum SkypeMessageType {
	mtypUnknown = -1,
	mtypAuthRequest = 0,
	mtypText = 1,
	mtypContacts = 2
};

[
	export,
	v1_enum,
	uuid("D6E5321C-982A-4470-9F50-D5E97280D2E0"),
	helpcontext(0106)
]
enum SkypeMessageStatus {
	msgUnknown = -1,
	msgSending = 0,
	msgSent = 1,
	msgFailed = 2,
	msgReceived = 3,
	msgRead = 4,
	msgIgnored = 5,
	msgQueued = 6
};

[
	export,
	v1_enum,
	uuid("127D4942-E520-4cfc-B5FD-B2FDFBA98782"),
	helpcontext(0107)
]
enum SkypeUserSex {
	usexUnknown = -1,
	usexNotSpecified = 0,
	usexMale = 1,
	usexFemale = 2
};

[
	export,
	v1_enum,
	uuid("1D72D739-69FB-48b0-9D2D-E9FA0E1108AE"),
	helpcontext(0108)
]
enum SkypeBuddyStatus {
	budUnknown = -1,
	budNever = 0,
	budDeleted = 1,
	budPending = 2,
	budFriend = 3
};

[
	export,
	v1_enum,
	uuid("BD1F7281-2E21-4d6c-993E-2C21B1114726"),
	helpcontext(0109)
]
enum SkypeOnlineStatus {
	olsUnknown = -1,
	olsOffline = 0,
	olsOnline = 1,
	olsAway = 2,
	olsNotAvail = 3,
	olsDoNotDisturb = 4,
	olsInvisible = 5,
	olsSkypeMe = 6,
	olsLoggedOut = 7
};

[
	export,
	v1_enum,
	uuid("8DD2EDDA-C8FE-4c37-8B49-FE04825C8206"),
	helpcontext(0110)
]
enum SkypeChatStatus {
	chatUnknown = -1,
	chatLegacyDialog = 0,
	chatDialog = 1,
	chatMultiNeedAccept = 2,
	chatMultiSubscribed = 3
};

[
	export,
	v1_enum,
	uuid("5F10C769-6EF1-4495-9E12-B37ED9C311D4"),
	helpcontext(0111)
]
enum SkypeChatMessageStatus {
	chatMsgUnknown = -1,
	chatMsgSending = 0,
	chatMsgSent = 1,
	chatMsgReceived = 2,
	chatMsgRead = 3
};

[
	export,
	v1_enum,
	uuid("2315D3E3-C08A-4a5d-8A87-5D9439DCBFFC"),
	helpcontext(0112)
]
enum SkypeChatMessageLeftReason {
	chatMsgLeftUnknown = -1,
	chatMsgLeftUserNotFound = 0,
	chatMsgLeftUserIncapable = 1,
	chatMsgLeftAdderMustBeFriend = 2,
	chatMsgLeftAdderMustBeAuthorized = 3,
	chatMsgLeftDeclineAdd = 4,
	chatMsgLeftUnsubscribe = 5
};

[
	export,
	v1_enum,
	uuid("380437AD-6069-4a9f-B53F-9FDB9D7FF6C6"),
	helpcontext(0113)
]
enum SkypeCurrencyType {
	currUnknown = -1,
	currEuroCents = 0
};

[
	export,
	v1_enum,
	uuid("D700390A-F14B-4275-9FF7-5E19802BEF3D"),
	helpcontext(0114)
]
enum SkypeChatMessageType {
	chatMsgTypeUnknown = -1,
	chatMsgTypeSetTopic = 0,
	chatMsgTypeSaid = 1,
	chatMsgTypeAddedMembers = 2,
	chatMsgTypeSawMembers = 3,
	chatMsgTypeCreatedChatWith = 4,
	chatMsgTypeLeft= 5
};

[
	export,
	v1_enum,
	uuid("FCDD7B32-D901-4933-8C5D-1B380BDE3C96"),
	helpcontext(0115)
]
enum SkypeVoiceMailType {
	vmTypeUnknown = -1,
	vmTypeIncoming = 0,
	vmTypeDefaultGreeting = 1,
	vmTypeCustomGreeting = 2,
	vmTypeOutgoing = 3
};

[
	export,
	v1_enum,
	uuid("0B7F55D7-ED68-4e09-91A4-0F20DB0E186F"),
	helpcontext(0116)
]
enum SkypeVoiceMailStatus {
	vmStatusUnknown = -1,
	vmStatusDownloading = 0,
	vmStatusDownloaded = 1,
	vmStatusUnplayed = 2,
	vmStatusBuffering = 3,
	vmStatusPlaying = 4,
	vmStatusPlayed = 5,
	vmStatusBlank = 6,
	vmStatusRecording = 7,
	vmStatusRecorded = 8,
	vmStatusUploading = 9,
	vmStatusUploaded = 10,
	vmStatusDeleting = 11,
	vmStatusFailed = 12
};

[
	export,
	v1_enum,
	uuid("57181448-1317-411c-B064-6F2E872F3BC0"),
	helpcontext(0117)
]
enum SkypeVoiceMailFailure {
	vmFailureUnknown = -1,
	vmFailureMisc = 0,
	vmFailureConnect = 1,
	vmFailureNoPrivilege = 2,
	vmFailureNoSuchVoiceMail = 3,
	vmFailureReadError = 4,
	vmFailureWriteError = 5,
	vmFailureRecording = 6,
	vmFailurePlayback = 7
};

[
	export,
	v1_enum,
	uuid("4D1434DB-6F9F-4533-ACAF-B712F9F54F81"),
	helpcontext(0118)
]
enum SkypeClearCallHistory {
	clrAllCalls = 0,
	clrMissedCalls = 1,
	clrIncomingCalls = 2,
	clrOutgoingCalls = 3
};

[
	export,
	v1_enum,
	uuid("A3678CC3-AC6E-4ffb-A8B5-FA57B97D4BF4"),
	helpcontext(0119)
]
enum SkypeNoAnswerAction {
	noAnsUnknown = -1,
	noAnsReject = 0,
	noAnsForward = 1,
	noAnsVoicemail = 2
};

[
	export,
	v1_enum,
	uuid("EEC83F5F-3413-4c9e-BACA-295A76125CD8"),
	helpcontext(0120)
]
enum SkypeOptionsPage {
	pgGeneral = 0,
	pgPrivacy = 1,
	pgNotifications = 2,
	pgSoundAlerts = 3,
	pgSoundDevices = 4,
	pgHotKeys = 5,
	pgConnection = 6,
	pgVoiceMail = 7,
	pgCallForward = 8,
	pgVideo = 9,
	pgAdvanced = 10
};

[
	export,
	v1_enum,
	uuid("E8EA86FF-4282-4bf1-B371-EDDA7F97D5B7"),
	helpcontext(0121)
]
enum SkypeRingtone {
	rngRingtone = 1101,
	rngRingback = 1102,
	rngBusy = 1103,
	rngDialing = 1104,
	rngConnecting = 1105,
	rngResume = 1202,
	rngHangup = 1203,
	rngIncomingMessage = 1204,
	rngOnlineAlert = 1205
};
