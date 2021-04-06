#include "AudioManager.h"

//--------------------------------------------------------------
void AudioManager::loadSamples() {

    //==================LOAD-SAMPLES-HERE===================//
    metronome.load(ofToDataPath("./audio/samples/rim.wav"));

    hitHi.load(ofToDataPath("./audio/samples/synthSpace_hi.wav"));
    hitMid.load(ofToDataPath("./audio/samples/synthSpace_mid.wav"));
    hitLow.load(ofToDataPath("./audio/samples/synthSpace_low.wav"));

    hitSub.load(ofToDataPath("./audio/samples/subAtmosphere.wav"));

    //=====================LOAD-LOOPS=======================//
    atmos.load(ofToDataPath("./audio/loops/atmosphere.wav"));

    keyArpLoop.load(ofToDataPath("./audio/loops/keyArploop.wav"));
    keyHi.load(ofToDataPath("./audio/loops/keysLoopHi.wav"));
    keyLow.load(ofToDataPath("./audio/loops/keysLoopLow.wav"));

}

//--------------------------------------------------------------
void AudioManager::envelopeSetup() {

    hitHiEnv.setAttack(0.2);
    hitHiEnv.setDecay(5);
    hitHiEnv.setSustain(5);
    hitHiEnv.setRelease(15000);

    hitMidEnv.setAttack(0.2);
    hitMidEnv.setDecay(5);
    hitMidEnv.setSustain(5);
    hitMidEnv.setRelease(15000);

    hitLowEnv.setAttack(0.2);
    hitLowEnv.setDecay(5);
    hitLowEnv.setSustain(5);
    hitLowEnv.setRelease(15000);

    hitSubEnv.setAttack(0.2);
    hitSubEnv.setDecay(5);
    hitSubEnv.setSustain(5);
    hitSubEnv.setRelease(15000);

}

//--------------------------------------------------------------
void AudioManager::guiSetup() {
    gui.setup();

    gui.add(cellSpeed.set("cell Speed", 42.662, 0, 60));
    gui.add(cellRadius.set("cell radius", 50, 0, 200));



    gui.add(masterGain.set("master gain", 1, 0, 3));



    gui.add(metGain.set("metronome gain", 0.0 /*0.5*/, 0, 1.0));

    gui.add(atmosGain.set("atmos gain", 0.3, 0, 1.0));

    gui.add(keyLoopGain.set("key loop gain", 0.4, 0, 1.0));
    gui.add(keyLoopPan.set("key loop Pan", 0.3, 0, 1.0));

    gui.add(keyHiGain.set("keys Hi gain", 0.15, 0, 1.0));
    gui.add(keyHiPan.set("keys Hi Pan", 0.8, 0, 1.0));

    gui.add(keyLowGain.set("keys Low gain", 0.18, 0, 1.0));
    gui.add(keyLowPan.set("keys Low Pan", 0.1, 0, 1.0));


    gui.add(synthLineGain.set("synth line gain", 0.01, 0, 0.05));
    gui.add(synthLinePan.set("synth line Pan", 0.05, 0, 1.0));


    gui.add(lowResFreq.set("lowRes filter freq", 4000, 0, 4000));
    gui.add(lowResQ.set("lowRes Filter Q", 0.5, 0, 4));


    gui.add(dryDelayMix.set("delay Wet/Dry Mix", 0.7, 0, 1));
    gui.add(delayFeedback.set("delay feedback", 0.5, 0, 0.9));
    gui.add(delayTime.set("delay time", 0.9, 0, 2));


    gui.add(dryReverbMix.set("reverb Wet/Dry Mix", 0.7, 0, 1));
    gui.add(reverbFeedback.set("reverb feedback", 0.95, 0, 0.99));

}

//--------------------------------------------------------------
void AudioManager::clockSetup() {

    //setup tempo and ticks per beat//
    //use for 2 bar samples
    clock.setTicksPerBeat(4);
    clock.setTempo(100);



}

//--------------------------------------------------------------
double AudioManager::playMetronome() {

    //---------SETUP-METRONOME-&-SEQUENCE-TRIGGER---------//
    met_out = metronome.playOnce(metSpeed);

    met_envOut = metEnv.ar(met_out, 0.1, 1.9, 1, metTrigger) * metGain;

    if (metTrigger == 1) {
        //set playback position to start of sample
        metronome.trigger();
    }

    return met_envOut;

}

//--------------------------------------------------------------
double AudioManager::playKeyLoop() {

    //---------SETUP-KEY-LOOP-&-SEQUENCE-TRIGGER-------//
    keyArp_out = keyArpLoop.playOnce();

    keyArp_envOut = keyArpEnv.ar(keyArp_out, 0.1, 1.0, 1, keyArpTrigger) * keyLoopGain;

    if (keyArpTrigger == 1) {
        //This sets the playback position to the start of a sample
        keyArpLoop.trigger();
    }

    return keyArp_envOut;
}

//--------------------------------------------------------------
double AudioManager::playKeyLoopHi() {

    //---------SETUP-KEY-LOOP-Hi-&-SEQUENCE-TRIGGER-------//
    keyHi_out = keyHi.playOnce();

    keyHi_envOut = keyHiEnv.ar(keyHi_out, 0.1, 1.0, 1, keyHiTrigger) * keyHiGain;

    if (keyHiTrigger == 1) {
        //This sets the playback position to the start of a sample
        keyHi.trigger();
    }

    return keyHi_envOut;
}

//--------------------------------------------------------------
double AudioManager::playKeyLoopLow() {

    //---------SETUP-KEY-LOOP-LOW-&-SEQUENCE-TRIGGER-------//
    keyLow_out = keyLow.playOnce();

    keyLow_envOut = keyLowEnv.ar(keyLow_out, 0.1, 1.0, 1, keyLowTrigger) * keyLowGain;

    if (keyLowTrigger == 1) {
        //This sets the playback position to the start of a sample
        keyLow.trigger();
    }

    return keyLow_envOut;
}

//--------------------------------------------------------------
void AudioManager::soundSetup(ofBaseApp* appPtr) {

    //=======MAXIM-SOUND-SETUP=========//
    sampleRate = 44100;
    bufferSize = 512;
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    //=====================================================//
    ofSoundStreamSetup(2, 0, appPtr, sampleRate, bufferSize, 4);
    /* this has to happen at the end of setup - it switches on the DAC */
}


AudioManager::~AudioManager()
{
    ofSoundShutdown();
    ofSoundStopAll();
    ofSoundStreamClose(); // <--- otherwise exceptions on exit
}

//===============================================================//
void AudioManager::setup(ofBaseApp* appPtr) {

    //=======OF-SETUP======//
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofBackground(0, 0, 0);
    //====================//


    //REVERB-SETUP
    for (int i = 0; i < 20; i++) {
        reverbTimes.push_back(ofRandom(0.02, 0.05));
    }

    //============================================//
    loadSamples(); //load samples
    envelopeSetup();
    clockSetup();  //setup tempo and ticks per beat
    guiSetup();    //setup gui
    soundSetup(appPtr);  //run at the end of setup!
    //============================================//
}

//--------------------------------------------------------------
void AudioManager::audioOut(float* output, int bufferSize, int nChannels) {

    stereoMix[0] = 0;
    stereoMix[1] = 0;

    lowResFilterMix[0] = 0;
    lowResFilterMix[1] = 0;

    delayMix[0] = 0;
    delayMix[1] = 0;

    stereoDelayOutput[0] = 0;
    stereoDelayOutput[1] = 0;

    reverbMix[0] = 0;
    reverbMix[1] = 0;

    reverbOutput[0] = 0;
    reverbOutput[1] = 0;

    masterStereoOutput[0] = 0;
    masterStereoOutput[1] = 0;


    for (int i = 0; i < bufferSize; i++) {

        //========================================//
        //-----CLOCK-&-SEQUENCER-SETUP-LOOPS------//
        //========================================//
        clock.ticker();     //start clock

        if (clock.tick) {   //iterate through sequences

            //-----------------------------------//
            metTrigger = metSeq[playHead % 16];
            metSpeed = metSpeedSeq[playHead % 16];
            //-----------------------------------//
            keyArpTrigger = keyArpSeq[playHead % 32];
            keyHiTrigger = keyHiSeq[playHead % 64];
            keyLowTrigger = keyLowSeq[playHead % 128];
            //-----------------------------------//

            //USE FOR TRIGERRING SYNTH LINE
            sBassTrigger = sBassLineTrigger[playHead % 16];
            if (sBassTrigger == 1) {

                sBassPitch = mtof.mtof(sBassLinePitch[sBassNote]);
                sBassNote++;

                if (sBassNote > 5) {
                    sBassNote = 0;
                }
            }


            playHead++;//iterate the playhead
        }
        //========================================//
        //========================================//
        //cell animation sin
        cellSinOut = cellSin.sinewave(cellSpeed);

        //synth arp sound
        sBass_envOut = sBassEnv.ar(sBass.sinewave(sBassPitch), 0.05, 0.99, 5000, sBassTrigger) * synthLineGain;


        //---------ATMOSPHERE-LOOP-PLAY------//
        atmos_out = atmos.play() * atmosGain;
        //===================================//

        //===================================//
        //---------SAMPLE-HITS---------------//
        //===================================//
        hitHi_out = hitHi.playOnce() * 0.8;
        hitHi_envOut = hitHiEnv.adsr(hitHi_out, hitHiEnv.trigger);
        hitHiStereo.stereo(hitHi_envOut, hitHiMix, 0.2);
        //---------------------------------//
        hitMid_out = hitMid.playOnce() * 0.8;
        hitMid_envOut = hitMidEnv.adsr(hitMid_out, hitMidEnv.trigger);
        hitMidStereo.stereo(hitMid_envOut, hitMidMix, 0.8);
        //---------------------------------//
        hitLow_out = hitLow.playOnce() * 0.8;
        hitLow_envOut = hitLowEnv.adsr(hitLow_out, hitLowEnv.trigger);
        hitLowStereo.stereo(hitLow_envOut, hitLowMix, 0.4);
        //===================================//
        hitSub_out = hitSub.playOnce() * 1.5;
        hitSub_envOut = hitSubEnv.adsr(hitSub_out, hitSubEnv.trigger);
        hitSubStereo.stereo(hitSub_envOut, hitSubMix, 0.3);
        //===================================//


        //----------------SEND-AUDIO-TO-STEREO-CHANNEL-DRY-MIX----------//
        atmosStereo.stereo(atmos_out, atmosStereoOut, (autoPanner.sinewave(0.01) + 1) / 2);
        keyArpStereo.stereo(playKeyLoop(), keyArpMix, keyLoopPan);
        keyHiStereo.stereo(playKeyLoopHi(), keyHiMix, keyHiPan);
        keyLowStereo.stereo(playKeyLoopLow(), keyLowMix, keyLowPan);
        sBassStereo.stereo(sBass_envOut, sBassMix, synthLinePan);
        //==============================================================//


        //=============================MIX===============================//
        stereoMix[0] = keyArpMix[0] + keyHiMix[0] + keyLowMix[0] + sBassMix[0] + playMetronome() + hitHiMix[0] + hitMidMix[0] + hitLowMix[0] + hitSubMix[0];
        stereoMix[1] = keyArpMix[1] + keyHiMix[1] + keyLowMix[1] + sBassMix[1] + playMetronome() + hitHiMix[1] + hitMidMix[1] + hitLowMix[1] + hitSubMix[1];
        //================================================================//




        //===================LOWRES=FILTER=MIX=================================//
        lowResFilterMix[0] = lowRes[0].lores(stereoMix[0], lowResFreq, lowResQ);
        lowResFilterMix[1] = lowRes[1].lores(stereoMix[1], lowResFreq, lowResQ);
        //=====================================================================//




        //==============================DELAY==========================================//
        delayMix[0] = delay[0].dl(lowResFilterMix[0], 44100 * delayTime, delayFeedback);
        delayMix[1] = delay[1].dl(lowResFilterMix[1], 44100 * delayTime, delayFeedback);
        //=============================================================================//

        //=======================DELAY-WET-DRY=MIX=======================
        stereoDelayOutput[0] = delayMix[0] * (1 - dryDelayMix) + (lowResFilterMix[0] * dryDelayMix);
        stereoDelayOutput[1] = delayMix[1] * (1 - dryDelayMix) + (lowResFilterMix[1] * dryDelayMix);
        //==============================================================//




        //========================================REVERB-CHANNEL======================================//
        for (int i = 0; i < 10; i++) {
            reverbMix[0] = reverbsL[i].dl(lowResFilterMix[0], 44100 * reverbTimes[i], reverbFeedback) / 10;
            reverbMix[1] = reverbsR[i].dl(lowResFilterMix[1], 44100 * reverbTimes[i], reverbFeedback) / 10;
        }
        //=============================================================================================//

        //=======================REVERB-WET/DRY-MIX===========================================//
        reverbOutput[0] = reverbMix[0] * (1 - dryReverbMix) + (lowResFilterMix[0] * dryReverbMix);
        reverbOutput[1] = reverbMix[1] * (1 - dryReverbMix) + (lowResFilterMix[1] * dryReverbMix);
        //====================================================================================//




        //============================================================//
        //------------------MASTER-OUTPUT-MIX-------------------------//
        masterStereoOutput[0] = stereoDelayOutput[0] + reverbOutput[0];
        masterStereoOutput[1] = stereoDelayOutput[1] + reverbOutput[1];
        //============================================================//

        //-----------------------------MASTER-OUTPUT-(STEREO)-----------------------------//
        output[i * nChannels] = (masterStereoOutput[0] + atmosStereoOut[0]) * masterGain;
        output[i * nChannels + 1] = (masterStereoOutput[1] + atmosStereoOut[1]) * masterGain;
        //================================================================================//
        //================================================================================//


        //==========RESET-SEQUENCER=TRIGGERS==========//
        metTrigger = 0;
        keyArpTrigger = 0;
        keyHiTrigger = 0;
        keyLowTrigger = 0;


        //use for drawing
//        soundBuffer[i] = (masterStereoOutput[0] + masterStereoOutput[1] +
//                          atmosStereoOut[0] + atmosStereoOut[1]) * 5;

        //pass sound ouput to array fo animation

        soundBuffer[i] = cellSinOut;
        soundBufMix[i] = +hitHi_envOut + hitMid_envOut + hitLow_envOut + hitSub_envOut;
    }

}


//--------------------------------------------------------------
void AudioManager::drawWaveform() {

    ofNoFill();
    ofSetColor(0, 200, 0);
    ofSetLineWidth(1.2);


    //    ofBeginShape();
    //    for(int i = 0; i < bufferSize; i++) {
    //        float x = ofMap(i, 0, bufferSize, 0, ofGetWidth());
    //
    //            ofVertex(x, ofGetHeight()*.5 + soundBuffer[i] * 200);
    //    }
    //    ofEndShape();


        //---------------------//

    ofPushMatrix();

    ofTranslate(WORLD_WIDTH / 2, WORLD_HEIGHT / 2, -1000);

    //ofBeginShape();
    for (int i = 0; i < 255; i++) {

        ofColor c = ofColor(0);
        float col = ofMap(soundBuffer[i], -0.8, 0.8, 100, 255);
        c.setHsb(col, i * 10, 200);

        //std::cout<<soundBuffer[i]<<std::endl;

        ofFill();
        ofSetColor(c);


        float x = (soundBuffer[i] + (10 * soundBufMix[i])) * cos(float(i)) * cellRadius;
        float y = (soundBuffer[i] + (10 * soundBufMix[i])) * sin(float(i)) * cellRadius;

        ofDrawCircle(x, y, 1);

    }
    //ofEndShape();

    ofPopMatrix();

}

//--------------------------------------------------------------
void AudioManager::update() {
    if (randomSampleTriggered) {
        if (ofGetFrameNum() % 30 == 0) {
            randomSampleTriggered = false;
        }
    }
}

//--------------------------------------------------------------
void AudioManager::draw() {
    drawWaveform();
}

void AudioManager::drawGUI(bool enable) {
    if (enable) gui.draw();
}

void AudioManager::playRandomSample() {

    if (!randomSampleTriggered) {
        randomSampleTriggered = true;
        //int r = (int)ofRandom(0, 3);
        switch (r)
        {
        case 0:
            hitHi.trigger();
            hitHiEnv.trigger = 1;
            break;
        case 1:
            hitMid.trigger();
            hitMidEnv.trigger = 1;
            break;
        case 2:
            hitLow.trigger();
            hitLowEnv.trigger = 1;
            break;
        }
        if (r < 2) r++;
        else r = 0;
    }
}

//--------------------------------------------------------------
void AudioManager::keyPressed(int key) {

    //use for interactions to trigger samples
    if (key == '1') {
        hitHi.trigger(); //resets sample position to 0
        hitHiEnv.trigger = 1; //sample is triggered if = 1
        //std::cout<<"true"<<std::endl;
    }

    if (key == '2') {
        hitMid.trigger(); //resets sample position to 0
        hitMidEnv.trigger = 1; //sample is triggered if = 1
        //std::cout<<"true"<<std::endl;
    }

    if (key == '3') {
        hitLow.trigger(); //resets sample position to 0
        hitLowEnv.trigger = 1; //sample is triggered if = 1
        //std::cout<<"true"<<std::endl;
    }

    if (key == '4') {
        hitSub.trigger(); //resets sample position to 0
        hitSubEnv.trigger = 1; //sample is triggered if = 1
        //std::cout<<"true"<<std::endl;
    }



}

//--------------------------------------------------------------
void AudioManager::keyReleased(int key) {
    //    hitHiEnv.trigger = 0;
    //    hitMidEnv.trigger = 0; //sample is triggered if = 1
    //    hitLowEnv.trigger = 0;
    //    hitSubEnv.trigger = 0;
}

//--------------------------------------------------------------
void AudioManager::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void AudioManager::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void AudioManager::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void AudioManager::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void AudioManager::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void AudioManager::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void AudioManager::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void AudioManager::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void AudioManager::dragEvent(ofDragInfo dragInfo) {

}
