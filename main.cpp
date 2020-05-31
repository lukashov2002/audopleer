/*==============================================================================
Play Sound Example
Copyright (c), Firelight Technologies Pty, Ltd 2004-2020.

This example shows how to simply load and play multiple sounds, the simplest
usage of FMOD. By default FMOD will decode the entire file into memory when it
loads. If the sounds are big and possibly take up a lot of RAM it would be
better to use the FMOD_CREATESTREAM flag, this will stream the file in realtime
as it plays.
==============================================================================*/
#include "fmod.hpp"
#include "common.h"
#include <string>

FMOD_RESULT play(FMOD::System *system, FMOD::Sound *sound, FMOD::Channel *channel, const std::string &path) {
    sound->release();
    auto result = system->createSound(Common_MediaPath("whole_lotta_love"), FMOD_DEFAULT, 0, &sound);
    result = sound->setMode(FMOD_LOOP_OFF);
    return result;
}

FMOD_RESULT _play_sound_(FMOD::System *system, FMOD::Sound *sound, FMOD::Channel *channel) {
    auto result = system->playSound(sound, 0, false, &channel);
    return result;
}

int FMOD_Main() {
    FMOD::System *system;
    FMOD::Sound *sound1, *sound2, *sound3;
    FMOD::ChannelControl *chanel_control = 0;
    FMOD::Channel *channel = 0;
    FMOD_RESULT result;
    unsigned int version;
    void *extradriverdata = 0;
    Common_Init(&extradriverdata);
    result = FMOD::System_Create(&system);
    result = system->getVersion(&version);
    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);

    result = system->createSound(Common_MediaPath("whole_lotta_love.mp3"), FMOD_DEFAULT, 0, &sound1);
    result = sound1->setMode(FMOD_LOOP_OFF);
    /* meow.mp3 has embedded loop points which automatically makes looping turn on, */
    /* so turn it off here.  We could have also just put FMOD_LOOP_OFF in the above CreateSound call. */

    /*result = system->createSound(Common_MediaPath("whole_lotta_love.mp3"), FMOD_SPEAKERMODE_STEREO, 0, &sound2);
    result = system->createSound(Common_MediaPath("whole_lotta_love.mp3"), FMOD_SPEAKERMODE_MONO, 0, &sound3);
    ERRCHECK(result);*/

    /*
        Main loop
    */
    int q = 0;
    do {
        Common_Update();
        q = 0;
        if (Common_BtnPress(BTN_MORE)) {
            bool paused;
            channel->getPaused(&paused);
            channel->setPaused(!paused);
        }

        if (Common_BtnPress(BTN_ACTION1)) {
            system->getChannelsPlaying(&q, NULL);
            if (q > 0) {
                channel->stop();
            }
            FMOD::Sound *sound;
            result = system->createSound(Common_MediaPath("meow.mp3"), FMOD_DEFAULT, 0, &sound);
            result = sound->setMode(FMOD_LOOP_OFF);
            //_play_sound_(system, sound, channel);
            result = system->playSound(sound, 0, false, &channel);
            ERRCHECK(result);
        }

        if (Common_BtnPress(BTN_ACTION2)) {
            result = system->getChannelsPlaying(&q, NULL);
            if (q > 0) {
                channel->stop();
            }
            result = system->playSound(sound1, 0, false, &channel);
            ERRCHECK(result);
        }

        if (Common_BtnPress(BTN_ACTION3)) {
            result = system->getChannelsPlaying(&q, NULL);
            if (q > 0) {
                channel->stop();
            }
            result = system->playSound(sound1, 0, false, &channel);
            ERRCHECK(result);
        }


        result = system->update();
        ERRCHECK(result);

        {
            unsigned int ms = 0;
            unsigned int lenms = 0;
            bool playing = false;
            bool paused = false;
            int channelsplaying = 0;

            if (channel) {
                FMOD::Sound *currentsound = 0;

                result = channel->isPlaying(&playing);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                    ERRCHECK(result);
                }

                result = channel->getPaused(&paused);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                    ERRCHECK(result);
                }

                result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                    ERRCHECK(result);
                }

                channel->getCurrentSound(&currentsound);
                if (currentsound) {
                    result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
                    if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) &&
                        (result != FMOD_ERR_CHANNEL_STOLEN)) {
                        ERRCHECK(result);
                    }
                }
            }

            system->getChannelsPlaying(&channelsplaying, NULL);

            Common_Draw("==================================================");
            Common_Draw("Play Sound Example.");
            Common_Draw("Copyright (c) Firelight Technologies 2004-2020.");
            Common_Draw("==================================================");
            Common_Draw("");
            Common_Draw("Press %s to play a mono sound (drumloop)", Common_BtnStr(BTN_ACTION1));
            Common_Draw("Press %s to play a mono sound (jaguar)", Common_BtnStr(BTN_ACTION2));
            Common_Draw("Press %s to play a stereo sound (swish)", Common_BtnStr(BTN_ACTION3));
            Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
            Common_Draw("");
            Common_Draw("Time %02d:%02d:%02d/%02d:%02d:%02d : %s", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100,
                        lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100,
                        paused ? "Paused " : playing ? "Playing" : "Stopped");
            Common_Draw("Channels Playing %d", channelsplaying);
            Common_Draw("lolololo");
        }

        Common_Sleep(50);
    } while (!Common_BtnPress(BTN_QUIT));

    result = sound1->release(); //shut down
    result = sound2->release();
    result = sound3->release();
    result = system->close();
    result = system->release();
    Common_Close();
    return 0;
}
