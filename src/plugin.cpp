#include "plugin.hpp"


Plugin* pluginInstance;


void init(Plugin* p) {
	pluginInstance = p;

	// Add modules here
	p->addModel(modelArpegiator);
	p->addModel(modelEG1);
	p->addModel(modelClampNAmp);
	p->addModel(modelEcho);
	p->addModel(modelFlipFlop);
	p->addModel(modelLFO);
	p->addModel(modelLFO_SNH);
	p->addModel(modelOneub);
	p->addModel(modelChannelX);
	p->addModel(modelMixerX);
	p->addModel(modelLadderVcf);
	p->addModel(modelJfilter);
	p->addModel(modelMelodizer);
	p->addModel(modelPortamento);
	p->addModel(modelQuantizer);
	p->addModel(modelRandombeat);
	p->addModel(modelSolar5);
	p->addModel(modelSolarv);
	p->addModel(modelVCA);
	p->addModel(modelXOR);
	// p->addModel(model);

	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
