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
    unsigned i_nbFeaturesExtracted;

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

    Index *index = new ORBIndex(indexPath, buildForwardIndex);
    ORBWordIndex *wordIndex = new ORBWordIndex(visualWordPath);
    FeatureExtractor *ife = new ORBFeatureExtractor((ORBIndex *)index, wordIndex);

    DIR *dir = NULL;
    struct dirent *ent = NULL;
    FeatureExtractor *fext = NULL;
    if ((dir = opendir ( "/tmp/img/" )) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        printf ("%s\n", ent->d_name);
        /*
         * open a file read all the contents in
         *
         * stat get the file size
         * allocate byte buffer
         * read it into the buffer
         * clear the buffer
         *
         * conInfo.uploadedData.data --- read, close
         *
         *   u_int32_t i_ret = featureExtractor->processNewImage(
            i_imageId, conInfo.uploadedData.size(), conInfo.uploadedData.data(),
            i_nbFeaturesExtracted);
         *
         */
        std::ifstream is (ent->d_name, std::ifstream::binary);
        if (is) {
          // get length of file:
          is.seekg (0, is.end);
          int length = is.tellg();
          is.seekg (0, is.beg);

          char * buffer = new char [length];

          std::cout << "Reading " << length << " characters... ";
          // read data as a block:
          is.read (buffer,length);

          if (is)
            std::cout << "all characters read successfully.";
          else
            std::cout << "error: only " << is.gcount() << " could be read";
          is.close();
          i++;
          /*
          u_int32_t i_ret = fext->processNewImage(
                     i, length, buffer,
                     i_nbFeaturesExtracted);
		  */
          delete[] buffer;
        }

      }
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
    }


    Searcher *is = new ORBSearcher((ORBIndex *)index, wordIndex);
    RequestHandler *rh = new RequestHandler(ife, is, index, authKey);

    s = new HTTPServer(rh, i_port, https);

    signal(SIGHUP, intHandler);
    signal(SIGINT, intHandler);

    s->run();

    cout << "Terminating Pastec." << endl;

    delete s;
    delete (ORBSearcher *)is;
    delete (ORBFeatureExtractor *)ife;
    delete (ORBIndex *)index;

    return 0;
}
