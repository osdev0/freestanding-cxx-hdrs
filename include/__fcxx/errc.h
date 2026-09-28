/* SPDX-License-Identifier: 0BSD */

/* Copyright (C) 2026 Mintsuki and contributors.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


/* The error conditions of <system_error>, which <charconv> also uses. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ERRC_H
#define __FREESTANDING_CXX_HDRS_FCXX_ERRC_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

namespace std {

/* The values are the library's own, not those of any system. The standard
 * has them be those of the error numbers of <cerrno>, which a freestanding
 * program has no values for, as they are those of the system a program
 * runs on, so there is no <cerrno>, deliberately: a program with error
 * numbers of its own defines them itself. What uses the enumerators, such as
 * <charconv>, only needs them to be distinct and not 0. They number the
 * enumerators in the order the standard lists them. The enumerators of
 * STREAMS are deprecated from C++23 on. */
enum class errc {
    address_family_not_supported = 1,
    address_in_use = 2,
    address_not_available = 3,
    already_connected = 4,
    argument_list_too_long = 5,
    argument_out_of_domain = 6,
    bad_address = 7,
    bad_file_descriptor = 8,
    bad_message = 9,
    broken_pipe = 10,
    connection_aborted = 11,
    connection_already_in_progress = 12,
    connection_refused = 13,
    connection_reset = 14,
    cross_device_link = 15,
    destination_address_required = 16,
    device_or_resource_busy = 17,
    directory_not_empty = 18,
    executable_format_error = 19,
    file_exists = 20,
    file_too_large = 21,
    filename_too_long = 22,
    function_not_supported = 23,
    host_unreachable = 24,
    identifier_removed = 25,
    illegal_byte_sequence = 26,
    inappropriate_io_control_operation = 27,
    interrupted = 28,
    invalid_argument = 29,
    invalid_seek = 30,
    io_error = 31,
    is_a_directory = 32,
    message_size = 33,
    network_down = 34,
    network_reset = 35,
    network_unreachable = 36,
    no_buffer_space = 37,
    no_child_process = 38,
    no_link = 39,
    no_lock_available = 40,
    no_message_available _FCXX_DEPRECATED("deprecated in C++23") = 41,
    no_message = 42,
    no_protocol_option = 43,
    no_space_on_device = 44,
    no_stream_resources _FCXX_DEPRECATED("deprecated in C++23") = 45,
    no_such_device_or_address = 46,
    no_such_device = 47,
    no_such_file_or_directory = 48,
    no_such_process = 49,
    not_a_directory = 50,
    not_a_socket = 51,
    not_a_stream _FCXX_DEPRECATED("deprecated in C++23") = 52,
    not_connected = 53,
    not_enough_memory = 54,
    not_supported = 55,
    operation_canceled = 56,
    operation_in_progress = 57,
    operation_not_permitted = 58,
    operation_not_supported = 59,
    operation_would_block = 60,
    owner_dead = 61,
    permission_denied = 62,
    protocol_error = 63,
    protocol_not_supported = 64,
    read_only_file_system = 65,
    resource_deadlock_would_occur = 66,
    resource_unavailable_try_again = 67,
    result_out_of_range = 68,
    state_not_recoverable = 69,
    stream_timeout _FCXX_DEPRECATED("deprecated in C++23") = 70,
    text_file_busy = 71,
    timed_out = 72,
    too_many_files_open_in_system = 73,
    too_many_files_open = 74,
    too_many_links = 75,
    too_many_symbolic_link_levels = 76,
    value_too_large = 77,
    wrong_protocol_type = 78,
};

} /* namespace std */

#endif
