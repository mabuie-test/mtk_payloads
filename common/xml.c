/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Shomy
 */

#include <yxml.h>
#include <stdarg.h>
#include <libc.h>
#include <types.h>
#include <xml.h>

#ifdef XML_ALLOC
// Todo: Make this less bad.
extern void *(*volatile malloc)(size_t size);

#endif

#define XML_STACK_SIZE 4096

#ifdef XML_ALLOC
static u8* g_xml_stack = NULL;
#else
static u8 g_xml_stack[XML_STACK_SIZE];
#endif

// Basically makes it so we can use stuff like da/arg/arg1 for compatibility
// with MXML
static void update_path(xml_parser_t *p, const char *elem, bool going_deeper) {
    if (going_deeper) {
        if (p->depth > 0) {
            strncat(p->path, "/", sizeof(p->path) - strlen(p->path) - 1);
        }
        strncat(p->path, elem, sizeof(p->path) - strlen(p->path) - 1);
        p->depth++;
    } else {
        char *last_slash = strrchr(p->path, '/');
        if (last_slash) {
            *last_slash = '\0';
        } else {
            p->path[0] = '\0';
        }
        p->depth--;
    }
}

const char *get_node_text(xml_parser_t *p, const char *path) {
    yxml_ret_t ret;
    bool in_target = false;
    size_t i = 0;

    if (!p || !p->xml || !path) {
        return NULL;
    }

    const char *xml = p->xml;
    char *out = p->value[p->value_idx % XML_VALUE_SLOTS];
    p->value_idx = (p->value_idx + 1) % XML_VALUE_SLOTS;

    #ifdef XML_ALLOC
    // I rather allocate then bloat the stack
    if (g_xml_stack == NULL) {
        g_xml_stack = malloc(XML_STACK_SIZE);
        if (!g_xml_stack) {
            return NULL;
        }
    }
    #endif

    // Reset parser
    yxml_init(&p->yxml, g_xml_stack, XML_STACK_SIZE);
    p->path[0] = '\0';
    p->depth = 0;
    out[0] = '\0';

    while (xml[i]) {
        ret = yxml_parse(&p->yxml, (int)(u8)xml[i]);
        if (ret < 0) return NULL;

        switch (ret) {
            case YXML_ELEMSTART:
                update_path(p, p->yxml.elem, true);
                if (strcmp(p->path, path) == 0) {
                    in_target = true;
                    out[0] = '\0';
                }
                break;
            case YXML_CONTENT:
                if (in_target) {
                    size_t len = strlen(out);
                    size_t data_len = strlen(p->yxml.data);
                    if (len + data_len < XML_VALUE_SIZE) {
                        strncat(out, p->yxml.data, XML_VALUE_SIZE - len - 1);
                    }
                }
                break;
            case YXML_ELEMEND:
                if (in_target && strcmp(p->path, path) == 0) {
                    yxml_eof(&p->yxml);
                    return out;
                }
                update_path(p, p->yxml.elem, false);
                break;
            default:
                break;
        }
        i++;
    }

    if (yxml_eof(&p->yxml) != YXML_OK) {
        return NULL;
    }

    if (in_target) {
        return out;
    }

    return NULL;
}

bool xml_check_nodes(xml_parser_t *p, ...) {
    va_list args;
    const char *path;

    if (!p || !p->xml) {
        return false;
    }

    va_start(args, p);
    while ((path = va_arg(args, const char *))) {
        if (get_node_text(p, path) == NULL) {
            va_end(args);
            return false;
        }
    }
    va_end(args);
    return true;
}
