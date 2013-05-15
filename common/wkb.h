/*
 * Copyright 2013 Luciad (http://www.luciad.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef GPKG_WKB_H
#define GPKG_WKB_H

#include "allocator.h"
#include "binstream.h"
#include "geomio.h"

/**
 * \addtogroup wkb Well-known binary I/O
 * @{
 */

/**
 * A Well-Known Binary writer. wkb_writer_t instances can be used to generate a WKB blob based on
 * any geometry source. Use wkb_writer_geom_consumer() to obtain a geom_consumer_t pointer that can be passed to
 * geomtery sources.
 */
typedef struct {
    /** @private */
    geom_consumer_t geom_consumer;
    /** @private */
    binstream_t stream;
    /** @private */
    size_t start[GEOM_MAX_DEPTH];
    /** @private */
    size_t children[GEOM_MAX_DEPTH];
    /** @private */
    int offset;
} wkb_writer_t;

/**
 * Initializes a Well-Known Binary writer.
 * @param writer the writer to initialize
 * @param allocator the memory allocator that should be used to allocate internal memory buffers
 * @return SQLITE_OK on success, an error code otherwise
 */
int wkb_writer_init( wkb_writer_t *writer, const allocator_t *allocator );

/**
 * Destroys a Well-Known Binary writer.
 * @param writer the writer to destroy
 */
void wkb_writer_destroy( wkb_writer_t *writer );

/**
 * Returns a Well-Known Binary writer as a geometry consumer. This function should be used
 * to pass the writer to another function that takes a geom_consumer_t as input.
 * @param writer the writer
 */
geom_consumer_t* wkb_writer_geom_consumer(wkb_writer_t *writer);

/**
 * Returns a pointer to the Well-Known Binary data that was written by the given writer. The length of the returned
 * buffer can be obtained using the wkb_writer_length() function.
 * @param writer the writer
 * @return a pointer to the Well-Known Binary data
 */
uint8_t* wkb_writer_getwkb( wkb_writer_t *writer );

/**
 * Returns the length of the buffer obtained using the wkb_writer_getwkb() function.
 * @param writer the writer
 * @return the length of the Well-Known Binary data buffer
 */
size_t wkb_writer_length( wkb_writer_t *writer );

/**
 * Parses a Well-Known Binary geometry from the given stream. The stream should be positioned at the start
 * of the WKB geometry.
 *
 * @param stream the stream containing the WKB geometry
 * @param consumer the geometry consumer that will receive the parsed geometry
 * @return SQLITE_OK on success, an error code otherwise
 */
int wkb_read_geometry(binstream_t *stream, const geom_consumer_t *consumer);

/**
 * Parses the header of a Well-Known Binary geometry from the given stream. The stream should be positioned at the start
 * of the WKB geometry. The data contained in the header is written to the header parameter.
 *
 * @param stream the stream containing the WKB geometry
 * @param[out] header the header to populate
 * @return SQLITE_OK on success, an error code otherwise
 */
int wkb_read_header(binstream_t *stream, geom_header_t *header);

/**
 * Populates a geometry envelope based on the coordinates of a Well-Known Binary geometry from the given stream. The
 * stream should be positioned at the start of the WKB geometry.
 *
 * @param stream the stream containing the WKB geometry
 * @param[out] envelope the envelope to populate
 * @return SQLITE_OK on success, an error code otherwise
 */
int wkb_fill_envelope(binstream_t *stream, geom_envelope_t *envelope);

/** @} */

#endif
