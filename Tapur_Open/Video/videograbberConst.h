#if !defined(AFX_VIDEOGRABBERCONST_H__3C2FC476_7972_4C08_989D_BE5A7A4046DD__INCLUDED_)
#define AFX_VIDEOGRABBERCONST_H__3C2FC476_7972_4C08_989D_BE5A7A4046DD__INCLUDED_

 extern CString cs;	// - for temporary ...

// const int MAX_PATH = 260;

 void SetOptionButtonByIndex(CDialog* Dialog, const int nIDStart, int Indx);
// void EnableOrDisableFrameControls(CWnd *FrameName, BOOL EnableChkBox);
 void EnableOrDisableControls(CWnd *dlg, const int *hControls, BOOL En);
 void AssignListToComboBox(CComboBox *Dest, CString Source, long Indx, long MaxIndxLimit=-1);
 void AddLog(CEdit *TextBox, const CString &Line);
 void LoadTextBox(CListBox *Dest, const CString &Source);
 bool IsNumeric(CEdit *edit, long &Number);
 CString ToCString(long z);

 struct TFileSearch
 {
	void *Handle;
	bool Opened;
	CString Path;
	WIN32_FIND_DATA Data;
 };

 void FileSearch_Close(TFileSearch &FileSearch);

 CString FileSearch_NextFile(
	CString SearchPath,	CString FileKind, 
	BOOL RestartFromFirstFile, TFileSearch &FileSearch);


enum TxCursors
{
    cr_Default = 0,
    cr_None = 1,
    cr_Arrow = 2,
    cr_cross = 3,
    cr_IBeam = 4,
    cr_Size = 5,
    cr_SizeNESW = 6,
    cr_SizeNS = 7,
    cr_SizeNWSE = 8,
    cr_SizeWE = 9,
    cr_UpArrow = 10,
    cr_HourGlass = 11,
    cr_Drag = 12,
    cr_NoDrop = 13,
    cr_HSplit = 14,
    cr_VSplit = 15,
    cr_MultiDrag = 16,
    cr_SQLWait = 17,
    cr_No = 18,
    cr_AppStart = 19,
    cr_Help = 20,
    cr_HandPoint = 21,
    cr_SizeAll = 22
};

enum TxCurrentState
{
    cs_Down = 0,
    cs_Preview = 1,
    cs_Recording = 2,
    cs_Playback = 3,
    cs_Reencoding = 4
};

enum TxFrameGrabberRGBFormat
{
    fgf_Default = 0,
    fgf_RGB32 = 1,
    fgf_RGB24 = 2,
    fgf_RGB565 = 3,
    fgf_RGB555 = 4,
    fgf_RGB8 = 5
};

enum TxTriState
{
    ts_Undefined = 0,
    ts_False = 1,
    ts_True = 2
};

enum TxMpegStreamType
{
    mpst_Default = 0,
    mpst_Program = 1,
    mpst_Program_DVD = 2,
    mpst_Program_DVD_MC = 3,
    mpst_Program_SVCD = 4,
    mpst_MPEG1 = 5,
    mpst_MPEG1_VCD = 6
};

//enum TxTunerInputType
//{
//    TunerInputCable = 0,
//    TunerInputAntenna = 1
//};

enum TxTunerMode
{
    tm_TVTuner = 0,
    tm_FMRadioTuner = 1,
    tm_AMRadioTuner = 2,
    tm_DigitalSatelliteTuner = 3
};

enum TxHeaderAttribute
{
    ha_Title = 0,
    ha_Description = 1,
    ha_Author = 2,
    ha_Copyright = 3,
    ha_AlbumArtist = 4,
    ha_AlbumTitle = 5,
    ha_Composer = 6,
    ha_ContentDistributor = 7,
    ha_Director = 8,
    ha_EncodingTime = 9,
    ha_Genre = 10,
    ha_Language = 11,
    ha_ParentalRating = 12,
    ha_Producer = 13,
    ha_Provider = 14,
    ha_ToolName = 15,
    ha_ToolVersion = 16,
    ha_Writer = 17
};

enum TxCameraControl
{
    cc_Pan = 0,
    cc_Tilt = 1,
    cc_Roll = 2,
    cc_Zoom = 3,
    cc_Exposure = 4,
    cc_Iris = 5,
    cc_Focus = 6
};

enum TxFrameCaptureDest
{
    fc_TBitmap = 0,
    fc_BmpFile = 1,
    fc_JpegFile = 2,
    fc_Clipboard = 3
};

enum TxCrossbarSelector
{
    cs_VideoCrossbar = 0,
    cs_AudioCrossbar = 1
};

//enum TxPin_Direction
//{
//    PINDIR_INPUT = 0,
//    PINDIR_OUTPUT = 1
//};

enum TxFrameInfoId
{
    fi_FrameNumber = 0,
    fi_DroppedFrameCount = 1,
    fi_SampleTime_Hour = 2,
    fi_SampleTime_Min = 3,
    fi_SampleTime_Sec = 4,
    fi_SampleTime_Hs = 5,
    fi_SampleTime_TotalMin = 6,
    fi_DVTimeCode_IsAvailable = 7,
    fi_DVTimeCode_Hour = 8,
    fi_DVTimeCode_Min = 9,
    fi_DVTimeCode_Sec = 10,
    fi_DVTimeCode_Ff = 11,
    fi_DVTimeCode_TrackNumber = 12,
    fi_DVDateTime_IsAvailable = 13,
    fi_DVDateTime_Year = 14,
    fi_DVDateTime_Month = 15,
    fi_DVDateTime_Day = 16,
    fi_DVDateTime_Hour = 17,
    fi_DVDateTime_Min = 18,
    fi_DVDateTime_Sec = 19
};

enum TxLogType
{
    e_add_filter = 0,
    e_add_source_filter = 1,
    e_audio_compressor_not_suitable = 2,
    e_bind_moniker_to_filter = 3,
    e_compressor_possibly_not_suitable = 4,
    e_create_instance = 5,
    e_ddraw_caps_not_suitable = 6,
    e_device_in_use_in_another_graph = 7,
    e_disk_full = 8,
    e_failed = 9,
    e_failed_to_allocate_recording_file = 10,
    e_failed_to_bind_codec = 11,
    e_failed_to_connect_crossbar_pin = 12,
    e_failed_to_connect_to_server = 13,
    e_failed_to_create_directory = 14,
    e_failed_to_create_file = 15,
    e_failed_to_create_temp = 16,
    e_failed_to_bind_frame_grabber = 17,
    e_failed_to_load_ASF_profile = 18,
    e_failed_to_load_ASF_profile_custom_file = 19,
    e_failed_to_load_set_of_bitmaps = 20,
    e_failed_to_set_image_overlay = 21,
    e_failed_to_set_logo = 22,
    e_failed_to_play_backwards = 23,
    e_failed_to_render_file = 24,
    e_failed_to_renew_recording_file = 25,
    e_failed_to_set_player_speed_ratio_with_audio = 26,
    e_failed_to_setup_network_streaming = 27,
    e_failed_to_start_preview = 28,
    e_failed_to_start_recording = 29,
    e_file_in_use = 30,
    e_file_name_not_specified = 31,
    e_file_not_found = 32,
    e_get_audio_format = 33,
    e_get_interface = 34,
    e_get_video_format = 35,
    e_graph_error = 36,
    e_graph_cant_run = 37,
    e_graph_must_be_restarted = 38,
    e_hw_deinterlace_not_supported = 39,
    e_incompatible_options = 40,
    e_index_out_of_range = 41,
    e_invalid_directory = 42,
    e_library_not_found = 43,
    e_load_filter = 44,
    e_no_audio_input_device = 45,
    e_no_device_available = 46,
    e_no_dialog = 47,
    e_no_stream_control = 48,
    e_no_tv_tuner = 49,
    e_no_device_selected = 50,
    e_no_video_input_device = 51,
    e_not_allowed_during_network_streaming = 52,
    e_not_assigned = 53,
    e_not_multiplexed_master = 54,
    e_not_previewing = 55,
    e_not_recording = 56,
    e_not_reencoding = 57,
    e_not_streaming = 58,
    e_out_of_memory = 59,
    e_pause_resume_disabled = 60,
    e_pin_not_found = 61,
    e_interface_not_assigned = 62,
    e_query_config_avi_mux = 63,
    e_reencoding = 64,
    e_recording_cannot_pause = 65,
    e_render_audio_stream = 66,
    e_render_video_stream = 67,
    e_must_restart_master = 68,
    e_recording_on_motion_failed = 69,
    e_sendtodv_device_index_out_of_bound = 70,
    e_sendtodv_deviceindex_and_videodevice_have_same_value = 71,
    e_sendtodv_failed_to_bind_dv_device = 72,
    e_set_filter_graph = 73,
    e_set_interleaving_mode = 74,
    e_set_master_stream = 75,
    e_set_output_compatibility_index = 76,
    e_set_output_file_name = 77,
    e_set_format = 78,
    e_start_preview_first = 79,
    e_stop_player_first = 80,
    e_stop_preview_first = 81,
    e_stop_recording_first = 82,
    e_stop_reencoding_first = 83,
    e_storage_path_read_only = 84,
    e_streaming_type_not_specified = 85,
    e_third_party_filter_already_inserted = 86,
    e_third_party_filter_error = 87,
    e_trace_log = 88,
    e_tv_command_not_allowed_during_tv_tuning = 89,
    e_tuner_input_not_selected = 90,
    e_TVideoGrabber_Filter_obsolete = 91,
    e_value_out_of_range = 92,
    e_video_compressor_not_suitable = 93,
    e_window_transparency_failed = 94,
    e_invalid_size = 95,
    e_invalid_window_handle = 96,
    e_tuner_mode_not_supported = 97,
    i_audio_device_associated_to_video_device = 98,
    i_begin_discovering_device = 99,
    i_binding_device_or_compressor = 100,
    i_discovering_device = 101,
    i_end_discovering_device = 102,
    i_preallocated_file_size_large_enough = 103,
    i_preallocated_file_size_changed = 104,
    i_preallocated_file_not_suitable = 105,
    i_streaming_to_publishing_point = 106,
    i_third_party_filter_inserted = 107,
    i_using_ASF_Profile = 108,
    i_recording_videosubtype = 109,
    i_ismpegstream = 110,
    i_new_recording_filename = 111,
    i_using_property_group = 112,
    i_streaming_client_connected = 113,
    i_streaming_client_disconnected = 114,
    i_refreshing_preview = 115,
    i_recording_on_motion = 116,
    i_window_found = 117,
    i_GPL_MPEG2_Decoder_recommended = 118,
    i_tuner_mode = 119,
    w_cannot_connect_thirdparty_filter = 120,
    w_cannot_connect_thirdparty_renderer = 121,
    w_cannot_instantiate_thirdparty_filter = 122,
    w_cannot_route_audio_crossbar = 123,
    w_cannot_use_color_key = 124,
    w_command_delayed = 125,
    w_does_not_apply_to_dv = 126,
    w_find_audio_device = 127,
    w_filter_does_not_save_properties = 128,
    w_frame_grabber_requires_CPU = 129,
    w_hold_recording = 130,
    w_information = 131,
    w_not_playing = 132,
    w_player_audio_should_be_disabled = 133,
    w_recording_cancelled_by_user = 134,
    w_can_pause_and_ASF_incompatible = 135,
    w_set_audio_format = 136,
    w_storage_path_on_network = 137,
    w_tv_tuner = 138,
    w_using_nearest_video_size = 139,
    w_divx_codec_not_installed = 140,
    w_intervideo_codec_does_not_support_debugger = 141,
    w_should_install_divx_codec = 142,
    w_device_partially_supported = 143,
    w_excessive_grid_size = 144,
    w_grid_too_large_for_dialog = 145,
    w_operation_may_lock = 146,
    w_audio_streaming_needs_audiorecording_property_enabled = 147,
    w_network_streaming_disabled = 148,
    w_server_lost_next_retry = 149,
    w_overlay_mixer_not_available = 150,
    w_network_streaming_change_requires_application_to_be_restarted = 151,
    w_standard_renderer_recommended = 152,
    w_window_transparency_and_recording_not_recommended = 153
};

enum TxTVChannelInfo
{
    tci_Channel = 0,
    tci_DefaultVideoFrequency = 1,
    tci_OverriddenVideoFrequency = 2,
    tci_TunerVideoFrequency = 3,
    tci_TunerAudioFrequency = 4,
    tci_Locked = 5
};

enum TxVMR9ImageAdjustment
{
    vmr9_Brightness = 0,
    vmr9_Contrast = 1,
    vmr9_Hue = 2,
    vmr9_Saturation = 3,
    vmr9_Alpha = 4
};

enum TxVUMeterSetting
{
    vu_Handle = 0,
    vu_WarningPercent = 1,
    vu_PeakPercent = 2,
    vu_BkgndColor = 3,
    vu_NormalColor = 4,
    vu_WarningColor = 5,
    vu_PeakColor = 6,
    vu_TickSize = 7,
    vu_TickInterval = 8,
    vu_NeedleThickness = 9
};

enum TxGraphState
{
    gs_Stopped = 0,
    gs_Paused = 1,
    gs_Running = 2
};

enum TxDialog
{
    dlg_VideoDevice = 0,
    dlg_VideoCompressor = 1,
    dlg_AudioCompressor = 2,
    dlg_StreamConfig = 3,
    dlg_VfwFormat = 4,
    dlg_VfwSource = 5,
    dlg_vfwDisplay = 6,
    dlg_VideoCrossbar = 7,
    dlg_AudioCrossbar = 8,
    dlg_TVTuner = 9,
    dlg_TVAudio = 10,
    dlg_AudioDevice = 11,
    dlg_NetShowConfig = 12,
    dlg_DScaler = 13,
    dlg_None = 14
};

enum TxVideoQuality
{
    vq_Brightness = 0,
    vq_Contrast = 1,
    vq_Hue = 2,
    vq_Saturation = 3,
    vq_Sharpness = 4,
    vq_Gamma = 5,
    vq_WhiteBalance = 6,
    vq_Gain = 7,
    vq_ColorEnable = 8,
    vq_BacklightCompensation = 9
};

enum TxVideoControl
{
    vc_FlipHorizontal = 0,
    vc_FlipVertical = 1,
    vc_ExternalTriggerEnable = 2,
    vc_Trigger = 3
};

enum TxFormatType
{
    ft_VideoInfo = 0,
    ft_VideoInfo2 = 1,
    ft_DvInfo = 2,
    ft_Mpeg1Video = 3,
    ft_Mpeg2Video = 4,
    ft_Mpeg1Stream = 5,
    ft_Mpeg2Stream = 6,
    ft_MpegStreamType = 7,
    ft_MpegCustom = 8,
    ft_WaveFormatEx = 9,
    ft_Mpeg1Audio = 10,
    ft_Mpeg2Audio = 11,
    ft_Mpeg3Audio = 12,
    ft_OGG = 13,
    ft_Unknown = 14
};

enum TxPlaylist
{
    pl_Add = 0,
    pl_Remove = 1,
    pl_Clear = 2,
    pl_Loop = 3,
    pl_NoLoop = 4,
    pl_Play = 5,
    pl_Stop = 6,
    pl_Next = 7,
    pl_Previous = 8,
    pl_SortAlpha = 9,
    pl_SortRevAlpha = 10,
    pl_Random = 11,
    pl_Sequential = 12
};

enum TxDVCommand
{
    dv_Play = 0,
    dv_Stop = 1,
    dv_Freeze = 2,
    dv_Thaw = 3,
    dv_Ff = 4,
    dv_Rew = 5,
    dv_Record = 6,
    dv_RecordFreeze = 7,
    dv_RecordStrobe = 8,
    dv_StepFwd = 9,
    dv_StepRev = 10,
    dv_ModeShuttle = 11,
    dv_PlayFastestFwd = 12,
    dv_PlaySlowestFwd = 13,
    dv_PlayFastestRev = 14,
    dv_PlaySlowestRev = 15
};

enum TxThirdPartyFilterList
{
    tpf_VideoSource = 0,
    tpf_VideoPreview = 1,
    tpf_VideoRecording = 2,
    tpf_AudioSource = 3,
    tpf_AudioRendering = 4,
    tpf_AudioRecording = 5,
    tpf_VideoRenderer = 6,
    tpf_AudioRenderer = 7
};

enum TxAlignment
{
    taLeftJustify = 0,
    taRightJustify = 1,
    taCenter = 2
};

enum TxApplicationPriority
{
    ap_default = 0,
    ap_idle = 1,
    ap_normal = 2,
    ap_high = 3,
    ap_realtime = 4
};

enum TxASFDeinterlaceMode
{
    adm_NotInterlaced = 0,
    adm_DeinterlaceNormal = 1,
    adm_DeinterlaceHalfSize = 2,
    adm_DeinterlaceHalfSizeDoubleRate = 3,
    adm_DeinterlaceInverseTelecine = 4,
    adm_DeinterlaceVerticalHalfSizeDoubleRate = 5
};

enum TxNetworkStreaming
{
    ns_Disabled = 0,
    ns_ASFDirectNetworkStreaming = 1,
    ns_ASFStreamingToPublishingPoint = 2
};

enum TxNetworkStreamingType
{
    nst_AudioVideoStreaming = 0,
    nst_VideoStreaming = 1,
    nst_AudioStreaming = 2
};

enum TxAudioFormat
{
    af_default = 0,
    af_8000_8b_1ch = 1,
    af_8000_8b_2ch = 2,
    af_8000_16b_1ch = 3,
    af_8000_16b_2ch = 4,
    af_11025_8b_1ch = 5,
    af_11025_8b_2ch = 6,
    af_11025_16b_1ch = 7,
    af_11025_16b_2ch = 8,
    af_16000_8b_1ch = 9,
    af_16000_8b_2ch = 10,
    af_16000_16b_1ch = 11,
    af_16000_16b_2ch = 12,
    af_22050_8b_1ch = 13,
    af_22050_8b_2ch = 14,
    af_22050_16b_1ch = 15,
    af_22050_16b_2ch = 16,
    af_32000_8b_1ch = 17,
    af_32000_8b_2ch = 18,
    af_32000_16b_1ch = 19,
    af_32000_16b_2ch = 20,
    af_44100_8b_1ch = 21,
    af_44100_8b_2ch = 22,
    af_44100_16b_1ch = 23,
    af_44100_16b_2ch = 24,
    af_48000_8b_1ch = 25,
    af_48000_8b_2ch = 26,
    af_48000_16b_1ch = 27,
    af_48000_16b_2ch = 28
};

enum TxAudioSource
{
    as_Default = 0,
    as_UseExternalAudio = 1
};

enum TxVuMeter
{
    vu_Disabled = 0,
    vu_Analog = 1,
    vu_Bargraph = 2
};

enum TxAutoFileName
{
    fn_Sequential = 0,
    fn_DateTime = 1
};

enum TxBorderStyle
{
    bsNone = 0,
    bsSingle = 1
};

enum TxCompressionMode
{
    cm_NoCompression = 0,
    cm_CompressOnTheFly = 1,
    cm_CompressAfterRecording = 2
};

enum TxCompressionType
{
    ct_Video = 0,
    ct_Audio = 1,
    ct_AudioVideo = 2
};

enum TxAspectRatio
{
    ar_Box = 0,
    ar_Pan = 1,
    ar_Stretch = 2
};

enum TxDVVideoFormat
{
    dvf_Default = 0,
    dvf_DVSD = 1,
    dvf_DVHD = 2,
    dvf_DVSL = 3
};

enum TxDVSize
{
    dv_Full = 0,
    dv_Half = 1,
    dv_Quarter = 2,
    dv_DC = 3
};

enum TxDVVideoStandard
{
    dvs_Default = 0,
    dvs_PAL = 1,
    dvs_NTSC = 2
};

enum TxFrameGrabber
{
    fg_BothStreams = 0,
    fg_PreviewStream = 1,
    fg_CaptureStream = 2,
    fg_Disabled = 3
};

enum TxJPEGPerformance
{
    jpBestQuality = 0,
    jpBestSpeed = 1
};

enum TxLogoLayout
{
    lg_Centered = 0,
    lg_Stretched = 1,
    lg_Repeated = 2,
    lg_TopLeft = 3,
    lg_TopRight = 4,
    lg_BottomLeft = 5,
    lg_BottomRight = 6
};

enum TxMultiplexedRole
{
    mr_NotMultiplexed = 0,
    mr_MultiplexedMosaic4 = 1,
    mr_MultiplexedMosaic16 = 2,
    mr_MultiplexedMaster = 3,
    mr_MultiplexedSlave = 4
};

enum TxNotificationMethod
{
    nm_Timer = 0,
    nm_Thread = 1
};

enum TxThreadPriority
{
    tpIdle = 0,
    tpLowest = 1,
    tpLower = 2,
    tpNormal = 3,
    tpHigher = 4,
    tpHighest = 5,
    tpTimeCritical = 6
};

enum TxRecordingMethod
{
    rm_AVI = 0,
    rm_ASF = 1,
    rm_SendToDV = 2
};

enum TxRawSampleCaptureLocation
{
    rl_SourceFormat = 0,
    rl_AfterCompression = 1
};

enum TxRecordingSize
{
    rs_Default = 0,
    rs_HalfSize = 1,
    rs_QuarterSize = 2
};

enum TxSyncPreview
{
    sp_Auto = 0,
    sp_Disabled = 1,
    sp_Enabled = 2
};

enum TxCardinalDirection
{
    cd_North = 0,
    cd_NorthEast = 1,
    cd_East = 2,
    cd_SouthEast = 3,
    cd_South = 4,
    cd_SouthWest = 5,
    cd_West = 6,
    cd_NorthWest = 7
};

enum TxTextOverlayAlign
{
    tf_Left = 0,
    tf_Center = 1,
    tf_Right = 2
};

enum TxFileSort
{
    fs_TimeAsc = 0,
    fs_TimeDesc = 1,
    fs_NameAsc = 2,
    fs_NameDesc = 3
};

enum TxVideoDeinterlacing
{
    di_Disabled = 0,
    di_HalfSize = 1,
    di_FullSize = 2,
    di_DScaler = 3
};

enum TxVideoRotation
{
    rt_0_deg = 0,
    rt_90_deg = 1,
    rt_180_deg = 2,
    rt_270_deg = 3,
    rt_0_deg_mirror = 4,
    rt_90_deg_mirror = 5,
    rt_180_deg_mirror = 6,
    rt_270_deg_mirror = 7
};

enum TxVideoRenderer
{
    vr_AutoSelect = 0,
    vr_VMR9 = 1,
    vr_VMR7 = 2,
    vr_StandardRenderer = 3,
    vr_OverlayRenderer = 4,
    vr_None = 5
};

enum TxVideoRendererExternal
{
    vre_None = 0,
    vre_Matrox_PRO = 1,
    vre_Decklink_SD = 2,
    vre_Decklink_Extreme = 3
};

enum TxVideoSource
{
    vs_VideoCaptureDevice = 0,
    vs_ScreenRecording = 1,
    vs_VideoFileOrURL = 2,
    vs_JPEGsOrBitmaps = 3,
    vs_VideoFromImages = 4
};

enum TxMouseButton
{
    mbLeft = 0,
    mbRight = 1,
    mbMiddle = 2
};


#endif // !defined(AFX_VIDEOGRABBERCONST_H__3C2FC476_7972_4C08_989D_BE5A7A4046DD__INCLUDED_)
