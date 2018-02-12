/*
 * Copyright (C) Nabto - All Rights Reserved.
 */
/**
 * @file
 * Derived constants based on the configuration
 */

#ifndef _UNABTO_CONFIG_DERIVED_H_
#define _UNABTO_CONFIG_DERIVED_H_

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Hardwired unabto constants and macros:
#define UNABTO_HEADER_MAX_SIZE                  26
#define UNABTO_WINDOW_PAYLOAD_DATA_SIZE         20  // Doesn't depend on the Streaming configuration parameters
#define UNABTO_WINDOW_PAYLOAD_MAX_SIZE          32

#if NABTO_ENABLE_UCRYPTO
#   define UNABTO_CRYPTO_PAYLOAD_SIZE(bytes)    (6 + 16*((bytes)/16 + 3))
#else
#   define UNABTO_CRYPTO_PAYLOAD_SIZE(bytes)    (6 + 2*((bytes)/2 + 2))
#endif


// Derived constants:
// First the control packet sizes:
#define UNABTO_INVITE_BS_PACKET_MAX_SIZE        (16+14+5+ NABTO_DEVICE_NAME_MAX_SIZE) // 35+
#define UNABTO_INVITE_GSP_PACKET_MAX_SIZE       (16+14+36+5+ NABTO_DEVICE_VERSION_MAX_SIZE +5+ NABTO_URL_OVERRIDE_MAX_SIZE +17+ NABTO_DEVICE_NAME_MAX_SIZE) // 98+
#define UNABTO_ATTACH_REQ_GSP_PACKET_SIZE       (16+10+36+118)    // 180
#define UNABTO_ATTACH_RSP_GSP_PACKET_SIZE       (16+13+17+8+118)  // 172
#define UNABTO_CONNECT_REQ_PACKET_MAX_SIZE      (16+21+5+ NABTO_CLIENT_ID_MAX_SIZE +150) // 192+
#define UNABTO_CONNECT_RSP_PACKET_SIZE          (16+12) // 28
#define UNABTO_ALIVE_REQ_SIZE                   (16+9)  // 25
#if NABTO_ENABLE_EVENTCHANNEL
#define UNABTO_ALIVE_RSP_MAX_SIZE               (16+8+8+ NABTO_EVENT_CHANNEL_MAX_SIZE) // 32+
#else
#define UNABTO_ALIVE_RSP_MAX_SIZE               (16+8+8) // 32
#endif

// find maximum of the values above, some are easily seen to be smaller
#if UNABTO_CONNECT_REQ_PACKET_MAX_SIZE > UNABTO_INVITE_GSP_PACKET_MAX_SIZE
#   define UNABTO_COMMUNICATION_BUFFER_SIZE     UNABTO_CONNECT_REQ_PACKET_MAX_SIZE
#else
#   define UNABTO_COMMUNICATION_BUFFER_SIZE     UNABTO_INVITE_GSP_PACKET_MAX_SIZE
#endif
#if UNABTO_ALIVE_RSP_MAX_SIZE > UNABTO_COMMUNICATION_BUFFER_SIZE
#   undef  UNABTO_COMMUNICATION_BUFFER_SIZE
#   define UNABTO_COMMUNICATION_BUFFER_SIZE     UNABTO_ALIVE_RSP_MAX_SIZE
#endif

// Then the data packets:
#if NABTO_REQUEST_MAX_SIZE > NABTO_RESPONSE_MAX_SIZE
#   define UNABTO_DIALOGUE_PAYLOAD_MAX_SIZE     UNABTO_CRYPTO_PAYLOAD_SIZE(NABTO_REQUEST_MAX_SIZE)
#else
#   define UNABTO_DIALOGUE_PAYLOAD_MAX_SIZE     UNABTO_CRYPTO_PAYLOAD_SIZE(NABTO_RESPONSE_MAX_SIZE)
#endif

#if NABTO_ENABLE_STREAM
#   if NABTO_ENABLE_MICRO_STREAM
#       define UNABTO_SACK_PAYLOAD_SIZE NP_PAYLOAD_SACK_MAX_BYTELENGTH
#   else
#       define UNABTO_SACK_PAYLOAD_SIZE 0
#   endif
#   if NABTO_STREAM_RECEIVE_SEGMENT_SIZE > NABTO_STREAM_SEND_SEGMENT_SIZE
#       define USER_STREAM_DATA_SIZE            NABTO_STREAM_RECEIVE_SEGMENT_SIZE
#   else
#       define USER_STREAM_DATA_SIZE            NABTO_STREAM_SEND_SEGMENT_SIZE
#   endif

#   define UNABTO_STREAM_DATA_SIZE              (UNABTO_SACK_PAYLOAD_SIZE + UNABTO_WINDOW_PAYLOAD_DATA_SIZE + UNABTO_CRYPTO_PAYLOAD_SIZE(USER_STREAM_DATA_SIZE))
#   define UNABTO_STREAM_CTRL_SIZE              (UNABTO_SACK_PAYLOAD_SIZE + UNABTO_WINDOW_PAYLOAD_MAX_SIZE  + UNABTO_CRYPTO_PAYLOAD_SIZE(0))

#   if UNABTO_STREAM_DATA_SIZE < UNABTO_STREAM_CTRL_SIZE
#       define UNABTO_STREAM_PAYLOADS_MAX_SIZE  UNABTO_STREAM_CTRL_SIZE
#   else
#       define UNABTO_STREAM_PAYLOADS_MAX_SIZE  UNABTO_STREAM_DATA_SIZE
#   endif
#endif

#if NABTO_ENABLE_STREAM && (UNABTO_STREAM_PAYLOADS_MAX_SIZE > UNABTO_DIALOGUE_PAYLOAD_MAX_SIZE)
#   define UNABTO_PAYLOAD_MAX_SIZE              UNABTO_STREAM_PAYLOADS_MAX_SIZE
#else
#   define UNABTO_PAYLOAD_MAX_SIZE              UNABTO_DIALOGUE_PAYLOAD_MAX_SIZE
#endif

#define UNABTO_DATA_PACKET_MAX_SIZE             (UNABTO_HEADER_MAX_SIZE + UNABTO_PAYLOAD_MAX_SIZE)

// Find max of control packets and data packets:
#if UNABTO_DATA_PACKET_MAX_SIZE > UNABTO_COMMUNICATION_BUFFER_SIZE
#   undef  UNABTO_COMMUNICATION_BUFFER_SIZE
#   define UNABTO_COMMUNICATION_BUFFER_SIZE     UNABTO_DATA_PACKET_MAX_SIZE
#endif

#if NABTO_ENABLE_DNS
#define DNS_COMMUNICATION_BUFFER_SIZE 256
#if UNABTO_COMMUNICATION_BUFFER_SIZE < DNS_COMMUNICATION_BUFFER_SIZE
#   undef  UNABTO_COMMUNICATION_BUFFER_SIZE
#   define UNABTO_COMMUNICATION_BUFFER_SIZE     DNS_COMMUNICATION_BUFFER_SIZE
#endif
#endif

#if NABTO_ENABLE_DHCP
#define DHCP_COMMUNICATION_BUFFER_SIZE 548
#if UNABTO_COMMUNICATION_BUFFER_SIZE < DHCP_COMMUNICATION_BUFFER_SIZE
#   undef  UNABTO_COMMUNICATION_BUFFER_SIZE
#   define UNABTO_COMMUNICATION_BUFFER_SIZE     DHCP_COMMUNICATION_BUFFER_SIZE
#endif
#endif

#if NABTO_ENABLE_DNS_FALLBACK
#define DNS_FALLBACK_COMMUNICATION_BUFFER_SIZE 1500
#if UNABTO_COMMUNICATION_BUFFER_SIZE < DNS_FALLBACK_COMMUNICATION_BUFFER_SIZE
#   undef UNABTO_COMMUNICATION_BUFFER_SIZE
#   define UNABTO_COMMUNICATION_BUFFER_SIZE    DNS_FALLBACK_COMMUNICATION_BUFFER_SIZE
#endif
#endif

#ifdef NABTO_COMMUNICATION_BUFFER_SIZE
#   pragma message("The configuration parameter NABTO_COMMUNICATION_BUFFER_SIZE is deprecated and no longer used")
#   pragma message("Use the set of new parameters found in unabto_config_defaults")
#   error "NABTO_COMMUNICATION_BUFFER_SIZE no longer used"
#endif
#define NABTO_COMMUNICATION_BUFFER_SIZE         UNABTO_COMMUNICATION_BUFFER_SIZE



#ifdef NABTO_ENABLE_REMOTE_ACCESS
#   pragma message("The configuration parameter NABTO_ENABLE_REMOTE_ACCESS has been renamed and should no longer be used.")
#   pragma message("Use the NABTO_ENABLE_REMOTE_CONNECTION configuration parameter instead. Just rename it.")
#else
#define NABTO_ENABLE_REMOTE_ACCESS (NABTO_ENABLE_REMOTE_CONNECTION)
#endif
#define NABTO_ENABLE_LOCAL_ACCESS  (NABTO_ENABLE_LOCAL_CONNECTION || NABTO_ENABLE_LOCAL_ACCESS_LEGACY_PROTOCOL)
#define NABTO_ENABLE_CONNECTIONS   (NABTO_ENABLE_LOCAL_CONNECTION || NABTO_ENABLE_REMOTE_CONNECTION)

#endif //DOXYGEN_SHOULD_SKIP_THIS

#endif
