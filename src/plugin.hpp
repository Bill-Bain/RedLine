#pragma once
#include <rack.hpp>


using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
extern Model* modelEG1;
extern Model* modelFlipFlop;
extern Model* modelClampNAmp;
extern Model* modelEcho;
extern Model* modelOneub;
extern Model* modelChannelX;
extern Model* modelMixerX;
extern Model* modelLadderVcf;
extern Model* modelJfilter;
extern Model* modelMelodizer;
extern Model* modelPortamento;
extern Model* modelQuantizer;
extern Model* modelRandombeat;
extern Model* modelLFO_SNH;
extern Model* modelSolar5;
extern Model* modelVCA;
extern Model* modelSolarv;
extern Model* modelArpegiator;
extern Model* modelLFO;
extern Model* modelXOR;




struct BBSN : app::SvgSwitch {
	BBSN() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BBSN_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BBSN_1.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BBSN_2.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BBSN_3.svg")));
	}
};

struct BBSW : app::SvgSwitch {
	BBSW() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BBSW_si.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BBSW_tr.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BBSW_sa.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BBSW_sq.svg")));
	}
};

struct NK2 : app::SvgSwitch {
	NK2() {
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/NKK_0.svg")));
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/NKK_2.svg")));
	}
};


