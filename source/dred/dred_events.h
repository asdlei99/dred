// Copyright (C) 2019 David Reid. See included LICENSE file.

// Here I'm making each custom event an offset of DTK_EVENT_CUSTOM. This isn't actually needed, but it just makes
// me feel better inside knowing that dred's custom event IDs don't overlap with those of DTK.

#define DRED_EVENT_IPC_TERMINATOR   (DTK_EVENT_CUSTOM + 0)
#define DRED_EVENT_IPC_ACTIVATE     (DTK_EVENT_CUSTOM + 1)  /* This can be removed when the new activation system is working. */
#define DRED_EVENT_IPC_GET_WINDOW   (DTK_EVENT_CUSTOM + 2)
#define DRED_EVENT_IPC_OPEN         (DTK_EVENT_CUSTOM + 3)