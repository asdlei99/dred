// Copyright (C) 2016 David Reid. See included LICENSE file.

// These #defines enable us to load large files on Linux platforms. They need to be placed before including any headers.
#ifndef _WIN32
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif
#endif



// Standard headers.
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable:4091)   // 'typedef ': ignored on left of 'tagGPFIDL_FLAGS' when no variable is declared
#endif

// Platform headers.
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __linux__
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <fontconfig/fontconfig.h>
#endif

// Platform libraries, for simplifying MSVC builds.
#ifdef _WIN32
#if defined(_MSC_VER) || defined(__clang__)
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "comctl32.lib")
#endif
#endif


// External libraries.
#define DR_IMPLEMENTATION
#include "../../../../dr_libs/dr.h"

#define DR_2D_IMPLEMENTATION
#include "../../../dr_gui/dr_2d.h"

#define DR_TEXT_ENGINE_IMPLEMENTATION
#include "../external/dr_text_engine.h"

#define DR_IPC_IMPLEMENTATION
#include "../../../dr_ipc/dr_ipc.h"

#define GB_STRING_IMPLEMENTATION
#include "../external/gb_string.h"


#ifdef _MSC_VER
#define DRED_INLINE __forceinline
#else
#define DRED_INLINE inline
#endif


#define DRED_PIPE_NAME  "dred"


// dred header files.
#include "dred_autogenerated.h"
#include "dred_build_config.h"
#include "dred_types.h"
#include "dred_ipc.h"
#include "gui/dred_gui.h"
#include "gui/dred_scrollbar.h"
#include "gui/dred_tabbar.h"
#include "gui/dred_button.h"
#include "gui/dred_color_button.h"
#include "gui/dred_checkbox.h"
#include "gui/dred_tabgroup.h"
#include "gui/dred_tabgroup_container.h"
#include "gui/dred_textview.h"
#include "gui/dred_textbox.h"
#include "gui/dred_info_bar.h"
#include "gui/dred_cmdbar.h"
#include "dred_fs.h"
#include "dred_alias_map.h"
#include "dred_config.h"
#include "dred_accelerators.h"
#include "dred_shortcuts.h"
#include "dred_editor.h"
#include "dred_settings_editor.h"
#include "dred_highlighters.h"
#include "dred_text_editor.h"
#include "dred_font.h"
#include "dred_font_library.h"
#include "dred_image.h"
#include "dred_image_library.h"
#include "dred_menus.h"
#include "dred_about_dialog.h"
#include "dred_settings_dialog.h"
#include "dred_printing.h"
#include "dred_context.h"
#include "dred_platform_layer.h"
#include "dred_commands.h"
#include "dred_misc.h"
#include "dred_stock_themes.h"

// dred source files.
#include "dred_autogenerated.c"
#include "dred_ipc.c"
#include "gui/dred_gui.c"
#include "gui/dred_scrollbar.c"
#include "gui/dred_tabbar.c"
#include "gui/dred_button.c"
#include "gui/dred_color_button.c"
#include "gui/dred_checkbox.c"
#include "gui/dred_tabgroup.c"
#include "gui/dred_tabgroup_container.c"
#include "gui/dred_textview.c"
#include "gui/dred_textbox.c"
#include "gui/dred_info_bar.c"
#include "gui/dred_cmdbar.c"
#include "dred_fs.c"
#include "dred_alias_map.c"
#include "dred_config.c"
#include "dred_accelerators.c"
#include "dred_shortcuts.c"
#include "dred_editor.c"
#include "dred_settings_editor.c"
#include "dred_text_editor.c"
#include "dred_font.c"
#include "dred_font_library.c"
#include "dred_image.c"
#include "dred_image_library.c"
#include "dred_menus.c"
#include "dred_about_dialog.c"
#include "dred_settings_dialog.c"
#include "dred_printing.c"
#include "dred_context.c"
#include "dred_platform_layer.c"
#include "dred_commands.c"
#include "dred_misc.c"
#include "dred_stock_themes.c"
#include "dred_highlighters.c"

#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

bool dred_parse_cmdline__post_startup_files_to_server(const char* key, const char* value, void* pUserData)
{
    drpipe client = (drpipe)pUserData;

    if (key == NULL) {
        dred_ipc_post_message(client, DRED_IPC_MESSAGE_OPEN, value, strlen(value)+1);   // +1 for null terminator.
        return true;
    }

    return true;
}

int dred_main(dr_cmdline cmdline)
{
    bool tryUsingExistingInstance = false;
    if (dr_cmdline_key_exists(&cmdline, "newinstance")) {
        tryUsingExistingInstance = true;
    }

    // If an instance of dred is already running, we may want to use that one instead. We can know this by trying
    // to create a client-side pipe.
    if (tryUsingExistingInstance) {
        drpipe client;
        if (drpipe_open_named_client(DRED_PIPE_NAME, DR_IPC_WRITE, &client) == dripc_result_success) {
            printf("CLIENT\n");
            // If we get here it means there is a server instance already open and we want to use that one instead
            // of creating a new one. The first thing to do is notify the server that it should be activated.
            dred_ipc_post_message(client, DRED_IPC_MESSAGE_ACTIVATE, NULL, 0);

            // After activating the server we need to let it know which files to open.
            dr_parse_cmdline(&cmdline, dred_parse_cmdline__post_startup_files_to_server, client);
        } else {
            printf("SERVER\n");
        }
    }


    // The platform needs to be initialized first. In the case of Windows, this will register the window classes
    // and enable DPI awareness. Always make sure this is the first thing to be called.
    dred_platform_init();

    dred_context dred;
    if (!dred_init(&dred, cmdline)) {
        return -1;
    }

    int result = dred_run(&dred);

    dred_uninit(&dred);
    dred_platform_uninit();
    return result;
}

int main(int argc, char** argv)
{
    dr_cmdline cmdline;
    dr_init_cmdline(&cmdline, argc, argv);

    return dred_main(cmdline);
}

#ifdef _WIN32
#ifdef NDEBUG
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    dr_cmdline cmdline;
    dr_init_cmdline_win32(&cmdline, GetCommandLineA());

    return dred_main(cmdline);
}
#endif
#endif
