#include "QxtAVFile.h"
#include "QxtAVPlayer.h"
#include "QxtAVPlayer_p.h"
#include <QxtCore/QxtHyperMacros>

/**
\class QxtAVPlayer QxtAVPlayer

\ingroup media

\brief simple player using the QxtAVFile and SDL

example:
\code 
QxtAudioPlayer player;
player.play("foo.wav");
\endcode
*/

//------------------------------------------------------------------------------------



static void Callback (void * userData, Uint8 *stream, int size)
	{
	short *out = (short*)stream;
	int length = size/sizeof(short);
	QxtAVPlayerPrivate * player =  (QxtAVPlayerPrivate *)userData;
	if(player->avfile)
		{
		short a[size];
		short *pa=a;
		player->avfile->read(a, length);

		fortimes(length)
			{
 			(*out++) =(short)((*pa++)*player->volume_m);
			}
		}

	else
		fortimes(length)
			*out++=0;
	}


//------------------------------------------------------------------------------------


QxtAVPlayerPrivate::QxtAVPlayerPrivate()
	{
	avfile=NULL;
	samplerate=48000;
	volume_m=0.99;
	opened_m=false;
	}
		
//------------------------------------------------------------------------------------


QxtError QxtAVPlayerPrivate::play(QString file)
	{
	if (opened_m)
		{
		SDL_PauseAudio (1);
		delete avfile;
		avfile= new QxtAVFile();
		QXT_DROP_F(avfile->open(file));
		SDL_PauseAudio (0);
		}
	else
		{
		avfile= new QxtAVFile();
		QXT_DROP_F(avfile->open(file));
		QXT_DROP_F(open());
		}

	QXT_DROP_OK;
	}
//------------------------------------------------------------------------------------

QxtError QxtAVPlayerPrivate::open()
	{
	if (opened_m)	QXT_DROP(Qxt::LogicalError);
	if (!avfile)	QXT_DROP(Qxt::Bug);




	if(SDL_Init (SDL_INIT_AUDIO)<0)QXT_DROP(Qxt::SDLError);

	SDL_AudioSpec wanted_spec;
	wanted_spec.freq=avfile->samplerate();
	wanted_spec.format=AUDIO_S16SYS;
	wanted_spec.channels=2;
        wanted_spec.silence = 0;

	wanted_spec.samples=2048;
	wanted_spec.callback=Callback;
	wanted_spec.userdata=this;
	got_spec=wanted_spec;

	if(SDL_OpenAudio (&wanted_spec, NULL)<0)QXT_DROP(Qxt::SDLError);

	
	///unpause
	SDL_PauseAudio (0);

	opened_m=true;
	QXT_DROP_OK;
	}
//------------------------------------------------------------------------------------
QxtError QxtAVPlayerPrivate::close()	
	{
	if (!opened_m)	QXT_DROP(Qxt::NotInitialised);
	SDL_PauseAudio (1);
 	SDL_CloseAudio();
	if(avfile)delete(avfile);
	QXT_DROP_OK;
	}
//------------------------------------------------------------------------------------


QxtError QxtAVPlayerPrivate::setVolume(float v)
	{
	if (v>0.99)v=0.99;
	if (v<0.0)v=0.0;
	volume_m=v;
	QXT_DROP_OK;
	}

QxtError QxtAVPlayerPrivate::pause      (bool e)
	{
	if (!opened_m)	QXT_DROP(Qxt::NotInitialised);

	if (e)
	SDL_PauseAudio (1);
	else
	SDL_PauseAudio (0);
	QXT_DROP_OK;
	}



//--------------------------------interface----------------------------------------------------


QxtAVPlayer::QxtAVPlayer(QObject * parent):QObject(parent)
	{
	}
QxtAVPlayer::~QxtAVPlayer()
	{
	qxt_d().close();
	}

QxtAVFile * QxtAVPlayer::currentFile()
	{
	return 	qxt_d().avfile;
	}

QxtError QxtAVPlayer::play      (QString url)
	{
	return 	qxt_d().play(url);
	}

QxtError QxtAVPlayer::pause      (bool e)
	{
	return 	qxt_d().pause(e);
	}
QxtError QxtAVPlayer::setVolume (float v)
	{
	return 	qxt_d().setVolume(v);
	};


