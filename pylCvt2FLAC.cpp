#include <pyliaison.h>
#include <iostream>
#include <math.h>
#include <algorithm>

#include <sndfile.h>

// Not using this but whatever
bool ChangeExtension(std::string& strFile, std::string strExt)
{
    size_t ixDot = strFile.rfind('.');
    if (ixDot == std::string::npos)
    {
        std::cout << "Unable to find extension of file " << strFile << std::endl;
        return false;
    }
    strFile = strFile.substr(0, ixDot) + "." + strExt;
    return true;
}

// Converts some input sound file to mono flac
bool cvt2FLAC(std::string strFileNameIn, std::string strFileNameOut)
{
    // Open input file
    SF_INFO infoIn{0};
    const char * szFileIn = (const char *) strFileNameIn.c_str();
    SNDFILE * pFileIn = sf_open( szFileIn, SFM_READ, &infoIn );
    SNDFILE * pFileOut = nullptr;
    bool bSuccess = false;
    if (pFileIn)
    {
        // We want either mono or stereo (what if there were 3...)
        if (infoIn.channels & 3)
        {
			std::vector<int> vDataOut( infoIn.frames );
			bool bFloat = (infoIn.format & SF_FORMAT_SUBMASK) == SF_FORMAT_FLOAT;
			if ( bFloat )
			{
				std::vector<float> vDataIn( infoIn.frames * infoIn.channels );
				sf_readf_float( pFileIn, vDataIn.data(), infoIn.frames );
				// Do stereo to mono if needed
				if ( infoIn.channels == 2 )
				{
					for ( int i = 0; i < infoIn.frames; i++ )
					{
						vDataOut[i] = (int) (INT_MAX * .5f * (vDataIn[2 * i] + vDataIn[2 * i + 1]));
					}
				}
				else
				{
					for ( int i = 0; i < infoIn.frames; i++ )
					{
						vDataOut[i] = (int) (.9f * INT_MAX * vDataIn[i]);
					}
				}
			}
			else
			{
				std::vector<short> vDataIn( infoIn.frames * infoIn.channels );
				sf_readf_short( pFileIn, vDataIn.data(), infoIn.frames );
				// Do stereo to mono if needed
				if ( infoIn.channels == 2 )
				{
					for ( int i = 0; i < infoIn.frames; i++ )
					{
						vDataOut[i] = (short) (.707f * (vDataIn[2 * i] + vDataIn[2 * i + 1]));
					}
				}
			}
            // Open a signed short mono FLAC file
            SF_INFO infoOut = infoIn;
            infoOut.channels = 1;
            infoOut.format = SF_FORMAT_FLAC | SF_FORMAT_PCM_24;
            pFileOut = sf_open( (const char *) strFileNameOut.c_str(), SFM_WRITE, &infoOut );
            if (pFileOut)
            {
                // Write output data, verify what we wrote
                int nFramesW = sf_writef_int(pFileOut, vDataOut.data(), (int)vDataOut.size());
                bSuccess = (nFramesW == (int)vDataOut.size());
                sf_write_sync(pFileOut); // ?
            }
            else
            {
                std::cout << "Unable to create output file " << strFileNameOut << std::endl;
            }
        }
        else
        {
            std::cout << "Invalid input file format" << std::endl;
        }
    }

    // Clean up anything we opened
    for (SNDFILE * pFile : {pFileIn, pFileOut})
        if (pFile)
            sf_close(pFile);

    return bSuccess;
}

// Pass input file and output flac file name
int main( int argc, char ** argv )
{
	// We may get an exception from the interpreter if something is amiss
	try
	{
		// Create modules - this must be done
		// prior to initializing the interpreter
		// The name of the module can be whatever you like,
		// so long as you don't conflict with anything important
		pyl::ModuleDef * pModDef = pylCreateMod( pylSndFile );

		// Add a function to the module
		pylAddFnToMod( pModDef, cvt2FLAC );

		// Initialize the python interpreter
		pyl::initialize();

		// Create script, this will run the code I need it to
		pyl::Object obScript( argv[1] );

		// Shut down the interpreter
		pyl::finalize();

		return EXIT_SUCCESS;
	}
	// These exceptions are thrown when something in pyliaison
	// goes wrong, but they're a child of std::runtime_error
	catch ( pyl::runtime_error e )
	{
		std::cout << e.what() << std::endl;
		pyl::print_error();
		pyl::finalize();
		return EXIT_FAILURE;
	}
}
