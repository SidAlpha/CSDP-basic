mode(-1);
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2011 - DIGITEO

// This file is released under the 3-clause BSD license. See COPYING-BSD.


verboseMode = ilib_verbose();
ilib_verbose(0);

function main_builder()
    TOOLBOX_NAME  = "csdp";
    TOOLBOX_TITLE = "FOSSEE CSDP Toolbox";
    toolbox_dir   = get_absolute_file_path("builder.sce");

    // Check Scilab's version
    // =============================================================================

    try
        v = getversion("scilab");
    catch
        error(gettext("Scilab 5.3 or more is required."));
    end

    if v(1) < 5 & v(2) < 3 then
        // new API in scilab 5.3
        error(gettext("Scilab 5.3 or more is required."));
    end

    // Check modules_manager module availability
    // =============================================================================

    if ~isdef("tbx_build_loader") then
        error(msprintf(gettext("%s module not installed."), "modules_manager"));
    end

    // Build Macros, Source and Gateway
    tbx_builder_macros(toolbox_dir);
    tbx_builder_gateway(toolbox_dir);
    
    // Generate Help
    //help_from_sci('macros', 'help/en_US', 'demos');
    //tbx_builder_help(toolbox_dir);

    // Build Loader & Cleaner
    tbx_build_loader(TOOLBOX_NAME, toolbox_dir);
    tbx_build_cleaner(TOOLBOX_NAME, toolbox_dir);

endfunction
// =============================================================================
main_builder();
ilib_verbose(verboseMode);
clear main_builder; // remove main_builder on stack
// =============================================================================


