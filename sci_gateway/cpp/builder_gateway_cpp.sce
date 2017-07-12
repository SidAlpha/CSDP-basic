// This file is released under the 3-clause BSD license. See COPYING-BSD.

function builder_gw_cpp()

	if isdef("TOOLBOX_NAME") then
		LIBRARY_NAME = TOOLBOX_NAME + "_cpp";
	else
		LIBRARY_NAME = "FOSSEE_CSDP_Toolbox_cpp";
	end
    GATEWAY_NAME = TOOLBOX_NAME + '_cpp';

    CUR_DIR = get_absolute_file_path('builder_gateway_cpp.sce');

    THIRD_PARTY_DIR = CUR_DIR + '/../../thirdparty';

    INC_DIR = [THIRD_PARTY_DIR + '/linux' + '/include'];

    [a, opt] = getversion();
    ARCH = opt(2);
LIB_DIR = [THIRD_PARTY_DIR + '/linux' + '/lib/'];

    LIB_SDP = [LIB_DIR + '/libsdp'];

 
    FUNC_NAMES = [
                    'csdp','csdp'
                ];

    FILES = [
				"sci_iofunc.cpp",
                'csdp.cpp'
            ];

    CXX_FLAGS = ['-std=c++0x -D__USE_DEPRECATED_STACK_FUNCTIONS__ -w -fpermissive -I''' + CUR_DIR + ''' -I''' + INC_DIR + ''' -Wl,-rpath=''' + LIB_DIR + ''' '];
    LD_FLAGS = ['-L''' + LIB_SDP  + '''  -lrt -lpthread'];
    tbx_build_gateway(GATEWAY_NAME, FUNC_NAMES, FILES, CUR_DIR, [], LD_FLAGS, CXX_FLAGS);

endfunction

builder_gw_cpp();
clear builder_gw_cpp; // remove builder_gw_cpp on stack
