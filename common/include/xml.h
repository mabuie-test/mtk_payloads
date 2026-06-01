/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Shomy
 */

#ifndef DA_XML_XML_H
#define DA_XML_XML_H

#include <types.h>
#include <libc.h>
#include <yxml.h>

#define XML_VALUE_SLOTS 8
#define XML_VALUE_SIZE  512

typedef struct {
    yxml_t yxml;
    const char *xml;
    char path[256];
    char value[XML_VALUE_SLOTS][XML_VALUE_SIZE];
    u8 value_idx;
    int depth;
} xml_parser_t;

const char *get_node_text(xml_parser_t *p, const char *path);
bool xml_check_nodes(xml_parser_t *p, ...);

#define XML_TEXT(parser, path) get_node_text(&(parser), (path))
#define XML_ATOULL(parser, path) atoull(XML_TEXT(parser, path))
#define XML_IS_YES(parser, path) (strncmp(XML_TEXT(parser, path), "yes", 3) == 0)

#define XML_LOAD(parser, xml, ...)                         \
    do {                                                    \
        (parser).xml = (xml);                               \
        (parser).value_idx = 0;                             \
        if (!xml_check_nodes(&(parser), __VA_ARGS__)) {     \
            printf("xml: missing required nodes\n");       \
            return STATUS_ERR;                              \
        }                                                   \
        (parser).value_idx = 0;                             \
    } while (0)

#endif //DA_XML_XML_H
