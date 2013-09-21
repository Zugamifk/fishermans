void
audiosystem_error
(
	audiosystem* as,
	errorlog* log,
	char* str
)
{
	char title[1024];
	sprintf(title, "AUDIO SYSTEM \"%s\" -- GENERAL", (char*)(as->name));
	
	errorlog_logdef(log, title, str);
}

bool
audiosystem_resultok
(
	audiosystem* as,
	errorlog* log,
	AUDIO_ERROR err
)
{

	if (err == FMOD_OK) return true;
	
	char title[1024];
	sprintf(title, "AUDIO SYSTEM \"%s\" -- FMOD", (char*)(as->name));
	
	char string[1024];
	strcpy(string, FMOD_ErrorString(err));
	errorlog_logdef(log, title, string);
	
	return false;
}

audiosystem*
audiosystem_init
(
	char* name,
	errorlog* log
)
{
	audio_data_init();
	audiosystem* as = malloc(sizeof(audiosystem));
	AUDIO_ERROR err;
	
	as->name = malloc(strlen(name));
	strcpy(as->name, name);
	
	err = FMOD_System_Create(&(as->sys));
	if (!audiosystem_resultok(as, log, err)) goto abort;
	
	unsigned int version;
	err = FMOD_System_GetVersion(as->sys, &version);
	if (!audiosystem_resultok(as, log, err)) goto abort;
	if (version < FMOD_VERSION) {
		char str[1024];
		sprintf(str, "Old version of FMOD (%ud) installed. Library requires version FMOD %ud or later.", version, FMOD_VERSION);
		audiosystem_error(as, log, str);
	}
	
	int numdrivers;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS caps;
	err = FMOD_System_GetNumDrivers(as->sys, &numdrivers);
	if (!audiosystem_resultok(as, log, err)) goto abort;
	
	if (numdrivers == 0) {
		err = FMOD_System_SetOutput(as->sys, FMOD_OUTPUTTYPE_NOSOUND);
		if (!audiosystem_resultok(as, log, err)) goto abort;
	} else {
		err = FMOD_System_GetDriverCaps(as->sys, 0, &caps, 0, &speakermode);
		if (!audiosystem_resultok(as, log, err)) goto abort;
		
		err = FMOD_System_SetSpeakerMode(as->sys, speakermode);
		if (!audiosystem_resultok(as, log, err)) goto abort;
	}
	
	if (caps & FMOD_CAPS_HARDWARE_EMULATED) {
		err = FMOD_System_SetDSPBufferSize(as->sys, AUDIO_BUFFERSIZE, AUDIO_NUMBUFFERS);
		if (!audiosystem_resultok(as, log, err)) goto abort;
	}
	
	err = FMOD_System_Init(as->sys, 100, FMOD_INIT_NORMAL, 0);
	if (err == FMOD_ERR_OUTPUT_CREATEBUFFER) {
		err = FMOD_System_SetSpeakerMode(as->sys, FMOD_SPEAKERMODE_STEREO);
		if (!audiosystem_resultok(as, log, err)) goto abort;
		
		err = FMOD_System_Init(as->sys, 100, FMOD_INIT_NORMAL, 0);
	}
	if (!audiosystem_resultok(as, log, err)) goto abort;
	
	as->sounds = hashtable_init(0);
	
	return as;
	
abort:
	free(as->name);
	free(as->sys);
	free(as);
	return NULL;
}

void
audiosystem_addsound
(
	audiosystem* as,
	char* name,
	audiostream* sound
)
{
	hashtable_insert(as->sounds, name, sound);
}

void
audiosystem_playsound
(
	audiosystem* as,
	char* name
)
{
	audiostream* str = hashtable_get(as->sounds, name);
	if (as == NULL) return;
	
	FMOD_System_PlaySound(as->sys, FMOD_CHANNEL_FREE, str->fmodsound, false, NULL);
}

void
audiosystem_update
(
	audiosystem* as,
	double t,
	double dt
)
{
	//AUDIO_ERROR err;
	FMOD_System_Update(as->sys);
	//audiosystem_resultok(as, log, err);
}
