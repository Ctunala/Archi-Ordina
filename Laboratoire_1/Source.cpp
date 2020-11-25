//Laboratoire 1
//Réalisé par Stéphane Tremblay 
//Pour le cours 6GEI311 - Architecture des logiciels

#include <iostream>
#include<dshow.h>
#include <string>

#pragma comment(lib, "Strmiids.lib")

int main()
{
	//Initialisation de la library COM
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		std::cout << "Erreur, la librarie n'a pu etre initialisee";
	}

	//Création du Filter Graph Manager
	IGraphBuilder *pGraph = NULL;
	IMediaControl *pControl = NULL;
	IMediaEvent *pEvent = NULL;
	IMediaSeeking* pSeek = NULL;

	hr = CoCreateInstance(CLSID_FilterGraph, NULL,
		CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
	if (FAILED(hr))
	{
		std::cout << "Erreur, impossible de creer le Filter Graph Manager";
	}

	hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
	hr = pGraph->QueryInterface(IID_IMediaSeeking, (void**)&pSeek);

	//Création du graph
	hr = pGraph->RenderFile(L"Example.avi", NULL);

	if (SUCCEEDED(hr))
	{
		
		// Run the graph.
		hr = pControl->Run();

		if (SUCCEEDED(hr))
		{
			std::string choix;
			bool run = true;
			bool play = true;
			bool accelere = false;
			int delay = 10;
			long evCode;

			pEvent->WaitForCompletion(INFINITE, &evCode);
			while (run) {
				std::cout << "Menu" << std::endl;
				std::cout << "P - Play ou Pause" << std::endl;
				std::cout << "A - Avance acceleree" << std::endl;
				std::cout << "R - Retour au debut" << std::endl;
				std::cout << "Q - Quitter" << std::endl;

				std::cin >> choix;
				choix = toupper(choix[0]);

				if (choix == "P") //Bouton Play ou Pause
				{
					if (play)
					{
						pControl->Pause();
					}
					else 
					{
						pControl->Run();
					}
					play = !play;

				}
				else if(choix == "Q") //Bouton arrêt video
				{
					run = false;
				}
				else if (choix == "A") //Bouton changement de vitesse
				{
					if (accelere)
					{
						pSeek->SetRate(1.0);
					}
					else 
					{
						pSeek->SetRate(2.0);
					}
					accelere = !accelere;
				}
				else if (choix == "R")
				{
					REFERENCE_TIME rtNow = 0;
					hr = pSeek->SetPositions(
						&rtNow, AM_SEEKING_AbsolutePositioning,
						NULL, AM_SEEKING_NoPositioning);
				}
			}
		}
	}
	pControl->Release();
	pEvent->Release();
	pGraph->Release();
	pSeek->Release();
	CoUninitialize();

	return 0;
}