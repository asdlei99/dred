// Copyright (C) 2019 David Reid. See included LICENSE file.

// This control is focused on simplicity and lightweightness. Length is limited to 32-bit. You probably don't want to use this for large text editing.
//
// Features/Limitations:
//   - Single font
//   - Single text color (no syntax highlighting)
//   - Single selection area (no multi-select)
//   - Simple realloc() based memory management of text string

typedef enum
{
    dtk_textbox_mode_single_line/*,
    dtk_textbox_mode_multi_line*/
} dtk_textbox_mode;

typedef enum
{
    dtk_text_alignment_left,
    dtk_text_alignment_top,
    dtk_text_alignment_right,
    dtk_text_alignment_bottom,
    dtk_text_alignment_center
} dtk_text_alignment;

#define DTK_TEXTBOX(a) ((dtk_textbox*)(a))
struct dtk_textbox
{
    dtk_control control;
    dtk_textbox_mode mode;
    char* pTextUTF8;
    size_t textLen; /* In bytes */
    size_t textCap; /* In bytes */
    size_t selectionBeg;
    size_t selectionEnd;
    size_t caretPosInBytes; /* In UTF-8 code units (bytes). Always on the boundary of a glyph. */
    struct
    {
        dtk_font* pFont;
        dtk_color bgColor;
        dtk_color fgColor;
        dtk_color selectionColor;
        dtk_color borderColor;
        dtk_uint32 borderWidth;
        dtk_uint32 paddingLeft;
        dtk_uint32 paddingTop;
        dtk_uint32 paddingRight;
        dtk_uint32 paddingBottom;
        dtk_text_alignment alignmentHorz;
        dtk_text_alignment alignmentVert;
        dtk_uint32 caretWidth;
    } style;
};

dtk_result dtk_textbox_init(dtk_context* pTK, dtk_event_proc onEvent, dtk_control* pParent, dtk_textbox* pTextBox);
dtk_result dtk_textbox_uninit(dtk_textbox* pTextBox);
dtk_bool32 dtk_textbox_default_event_handler(dtk_event* pEvent);
dtk_result dtk_textbox_set_mode(dtk_textbox* pTextBox, dtk_textbox_mode mode);
dtk_textbox_mode dtk_textbox_get_mode(dtk_textbox* pTextBox);
dtk_result dtk_textbox_set_text_utf8(dtk_textbox* pTextBox, const char* pTextUTF8, size_t textLen);  // Set textLen to -1 for null-terminated. Clears the selection.
const char* dtk_textbox_get_text_utf8(dtk_textbox* pTextBox);
size_t dtk_textbox_get_text_length_utf8(dtk_textbox* pTextBox); // In bytes. Does not include null terminator.
dtk_result dtk_textbox_set_font(dtk_textbox* pTextBox, dtk_font* pFont);
dtk_font* dtk_textbox_get_font(dtk_textbox* pTextBox);
dtk_result dtk_textbox_set_background_color(dtk_textbox* pTextBox, dtk_color color);
dtk_color dtk_textbox_get_background_color(dtk_textbox* pTextBox);
dtk_result dtk_textbox_set_foreground_color(dtk_textbox* pTextBox, dtk_color color);
dtk_color dtk_textbox_get_foreground_color(dtk_textbox* pTextBox);
dtk_result dtk_textbox_set_border_color(dtk_textbox* pTextBox, dtk_color color);
dtk_color dtk_textbox_get_border_color(dtk_textbox* pTextBox);
dtk_result dtk_textbox_set_border_width(dtk_textbox* pTextBox, dtk_uint32 width);
dtk_uint32 dtk_textbox_get_border_width(dtk_textbox* pTextBox);
dtk_result dtk_textbox_set_padding(dtk_textbox* pTextBox, dtk_uint32 left, dtk_uint32 top, dtk_uint32 right, dtk_uint32 bottom);
dtk_result dtk_textbox_get_padding(dtk_textbox* pTextBox, dtk_uint32* pLeft, dtk_uint32* pTop, dtk_uint32* pRight, dtk_uint32* pBottom);
dtk_result dtk_textbox_set_text_alignment(dtk_textbox* pTextBox, dtk_text_alignment alignmentHorz, dtk_text_alignment alignmentVert);
dtk_result dtk_textbox_get_text_alignment(dtk_textbox* pTextBox, dtk_text_alignment* pAlignmentHorz, dtk_text_alignment* pAlignmentVert);
dtk_result dtk_textbox_xy_to_cp(dtk_textbox* pTextBox, dtk_int32 x, dtk_int32 y, size_t* cp);
dtk_result dtk_textbox_cp_to_xy(dtk_textbox* pTextBox, size_t cp, dtk_int32* x, dtk_int32* y);
dtk_result dtk_textbox_move_caret_to_xy(dtk_textbox* pTextBox, dtk_int32 x, dtk_int32 y);
dtk_result dtk_textbox_move_caret_to_cp(dtk_textbox* pTextBox, size_t cp);
dtk_rect dtk_textbox_get_caret_rect(dtk_textbox* pTextBox);