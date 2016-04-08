/*****************************************************************************
 * Copyright (C) 2014 Visualink
 *
 * Authors: Adrien Maglo <adrien@visualink.io>
 *
 * This file is part of Pastec.
 *
 * Pastec is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pastec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Pastec.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include <dirent.h>


#include <signal.h>

#include <httpserver.h>
#include <requesthandler.h>
#include <orb/orbfeatureextractor.h>
#include <orb/orbsearcher.h>
#include <orb/orbwordindex.h>
#include <featureextractor.h>
#include <cstdio>
#include <ctime>

using namespace std;

HTTPServer *s;

void intHandler(int signum) {
    (void)signum;
    s->stop();
}


void printUsage()
{
    cout << "Usage :" << endl
         << "./pastec [-p portNumber] [-i indexPath] [--forward-index] [--https] [--auth-key AuthKey] [-d directoryToLoadImagesFrom] visualWordList" << endl;
}


int main(int argc, char** argv)
{
    cout << "Pastec Index v0.0.1" << endl;

    if (argc < 2)
    {
        printUsage();
        return 1;
    }

#define EXIT_IF_LAST_ARGUMENT() \
    if (i == argc - 1)          \
    {                           \
        printUsage();           \
        return 1;    \
    }

    unsigned i_port = 4212;
    string visualWordPath;
    string indexPath(DEFAULT_INDEX_PATH);
    bool buildForwardIndex = false;
    string authKey("");
    bool https = false;
    string loadDir;
    string imageLoad;
    unsigned i_nbFeaturesExtracted;
    std::clock_t start;
    double duration;

    int i = 1;
    while (i < argc)
    {
        if (string(argv[i]) == "-p")
        {
            EXIT_IF_LAST_ARGUMENT()
            i_port = atoi(argv[++i]);
        }
        else if (string(argv[i]) == "-i")
        {
            EXIT_IF_LAST_ARGUMENT()
            indexPath = argv[++i];
        }
        else if (string(argv[i]) == "--auth-key")
        {
            EXIT_IF_LAST_ARGUMENT()
            authKey = argv[++i];
        }
        else if (string(argv[i]) == "-d")
        {
            EXIT_IF_LAST_ARGUMENT()
            loadDir = argv[++i];
        }
        else if (string(argv[i]) == "--https")
        {
            https = true;
        }
        else if (string(argv[i]) == "--forward-index")
        {
            buildForwardIndex = true;
        }
        else if (i == argc - 1)
        {
            visualWordPath = argv[i];
        }
        else
        {
            printUsage();
            return 1;
        }
        ++i;
    }

    ORBIndex *index = new ORBIndex(indexPath, buildForwardIndex);
    ORBWordIndex *wordIndex = new ORBWordIndex(visualWordPath);
    ORBFeatureExtractor *ife = new ORBFeatureExtractor(index, wordIndex);

    DIR *pDIR = NULL;
    struct dirent *entry;
    int image_id = 0;

    if ((pDIR = opendir(loadDir.c_str()))) {

        start = std::clock();
        while ((entry = readdir(pDIR))) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 )
                continue;
            imageLoad = loadDir.c_str();
            string fn = loadDir + "/" + entry->d_name;
            cout << fn << endl;
            std::ifstream is(fn, std::ios::binary | std::ios::ate);
            if (is.is_open()) {
                int length = is.tellg();
                is.seekg (0, std::ios::beg);

                // char *buffer = new char[length];
                std::cout << "Reading " << length << " characters... " << entry->d_name << " :: ";
                // read data as a block:
                std::vector<char> buffer(length);
                if (is.read(buffer.data(), length))
                    std::cout << "all characters read successfully.";
                else
                    std::cout << "error: /tmp/img/" << entry->d_name << " only " << is.gcount() << " could be read";

                u_int32_t i_ret = ife->processNewImage(
                        ++image_id, length, buffer.data(), i_nbFeaturesExtracted);
                std::cout << "AEW DEBUG done with processNewImage" << std::endl;
            }
        }
        closedir(pDIR);
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"printf: "<< duration <<'\n';
    delete ife;
    delete wordIndex;
    delete index;

#if 0
    Searcher *is = new ORBSearcher(&index, wordIndex);
    RequestHandler *rh = new RequestHandler(ife, is, (Index *)&index, authKey);

    s = new HTTPServer(rh, i_port, https);

    signal(SIGHUP, intHandler);
    signal(SIGINT, intHandler);

    s->run();
#endif

    cout << "Terminating Pastec." << endl;

    return 0;
}

// vim: set ts=4 sw=4 expandtab :
