.. libutil main documentation

.. include:: <isonum.txt>

.. |libutil| replace:: GomSpace Util Library

*********
|libutil|
*********

.. toctree::
   :maxdepth: 3
   

Introduction
============

The |libutil| contains a number of usefull utilities that can be used when developing A3200 applications.

The following sections desctibes the |libutil| structure and API.

Structure
=========

The |libutil| is structured as follows:

.. tabularcolumns:: |p{4.0cm}|p{12.0cm}|
.. table:: |libutil| structure

   ===========================  ==============================================
   **Folder**                   **Description**
   ===========================  ==============================================
   libutil/include              The include folder contains the |libutil| API header files
   libutil/src                  The src folder contains the |libutil| API source code files. See :ref:`libutil_api` below for further details
   libutil/doc                  The doc folder contains the source code for this documentation
   ===========================  ==============================================

.. _libutil_api:

API
===

The |libutil| includes API functions for 

* Time and clock functions
* Byte order handling
* Print and print formatting functions
* CRC and checksum functions
* Delay functions
* Compression & decompression functions (LZO)
* Hash, List, Linked List, and Array operation
* Linux specific functions

Time and clock functions
------------------------

.. _libutil_api_clock:

Clock
^^^^^
The clock module contains functions for getting and setting the Real Time Clock (RTC) and for getting the time since last boot.

The clock module uses the type ``timestamp_t`` for exchanging timestamps:

.. code-block:: c

   typedef struct {
      uint32_t tv_sec;
      uint32_t tv_nsec;
   } timestamp_t;

Getting the time as a local time timestamp. The elements tv_sec and tv_nsec represents the seconds and nano-seconds since 1970.

.. code-block:: c

   void clock_get_time(timestamp_t * time);

Setting the time from an UTC timestamp.The elements tv_sec and tv_nsec must represent the seconds and nano-seconds since 1970 as an UTC timestamp.

.. code-block:: c

   void clock_set_time(timestamp_t * utc_time);

Getting the time as a monotonic timestamp (i.e. seconds and nano-seconds since boot):

.. code-block:: c

   void clock_get_monotonic(timestamp_t * time);

Getting the nano-seconds since boot as 64 bit unsigned integer:

.. code-block:: c

   uint64_t clock_get_nsec(void);
   
   
Timestamp
^^^^^^^^^

The timestamp module contains function for added, subtracting, comparing and copying timestamps.
The timestamps are of type ``timestamp_t`` as described in :ref:`libutil_api_clock`.

.. code-block:: c

   /** Add two timestamps, return result in base */
   int timestamp_add(timestamp_t * base, timestamp_t * add);

   /** Subtract two timestamps, return result in base */
   int timestamp_diff(timestamp_t * base, timestamp_t * diff);
   
   /** Compare two timestamps, return 1 if test > base, otherwise 0 */
   int timestamp_ge(timestamp_t * base, timestamp_t * test);
   
   /** Copy timestamp from 'from' to 'to' */
   int timestamp_copy(timestamp_t * from, timestamp_t * to);

Byte order handling
-------------------

Byteorder
^^^^^^^^^

The byte order module contains functions for converting numbers from host to network byte order and vice verca.
The byte order module also contains functions for converting numbers from host to little/big endian byte order and vice verca. 

Host - Network byte order:

.. code-block:: c

   /** Convert 16-bit integer from host byte order to network byte order
    * @param h16 Host byte order 16-bit integer*/
   uint16_t util_htons(uint16_t h16);
   
   /** Convert 16-bit integer from host byte order to host byte order
    * @param n16 Network byte order 16-bit integer */
   uint16_t util_ntohs(uint16_t n16);
   
   /** Convert 32-bit integer from host byte order to network byte order
    * @param h32 Host byte order 32-bit integer */
   uint32_t util_htonl(uint32_t h32);
   
   /** Convert 32-bit integer from host byte order to network byte order
    * @param h32 Host byte order 32-bit integer */
   uint32_t util_ntohl(uint32_t n32);
   
   /** Convert 16-bit integer from host byte order to network byte order
    * @param h16 Host byte order 16-bit integer */
   uint16_t util_hton16(uint16_t h16);
   
   /** Convert 16-bit integer from host byte order to host byte order
    * @param n16 Network byte order 16-bit integer */
   uint16_t util_ntoh16(uint16_t n16);
   
   /** Convert 32-bit integer from host byte order to network byte order
    * @param h32 Host byte order 32-bit integer */
   uint32_t util_hton32(uint32_t h32);
   
   /** Convert 32-bit integer from host byte order to host byte order
    * @param n32 Network byte order 32-bit integer */
   uint32_t util_ntoh32(uint32_t n32);
   
   /** Convert 64-bit integer from host byte order to network byte order
    * @param h64 Host byte order 64-bit integer */
   uint64_t util_hton64(uint64_t h64);
   
   /** Convert 64-bit integer from host byte order to host byte order
    * @param n64 Network byte order 64-bit integer */
   uint64_t util_ntoh64(uint64_t n64);
   
   /** Convert float from network to host byte order
    * @param float in host byte order */
   float util_htonflt(float f);
   
   /** Convert float from network to host byte order
    * @param float in network byte order */
   float util_ntohflt(float f);
   
   /** Convert double from network to host byte order
    * @param d double in network byte order */
   double util_htondbl(double d);
   
   /** Convert double from network to host byte order
    * @param d double in network byte order */
   double util_ntohdbl(double d);
   
Host - Little/Big Endian byte order:

.. code-block:: c

   /** Convert 16-bit integer from host byte order to big endian byte order
    * @param h16 Host byte order 16-bit integer */
   uint16_t util_htobe16(uint16_t h16);
   
   /** Convert 16-bit integer from host byte order to little endian byte order
    * @param h16 Host byte order 16-bit integer */
   uint16_t util_htole16(uint16_t h16);
   
   /** Convert 16-bit integer from big endian byte order to little endian byte order
    * @param be16 Big endian byte order 16-bit integer */
   uint16_t util_betoh16(uint16_t be16);
   
   /** Convert 16-bit integer from little endian byte order to host byte order
    * @param le16 Little endian byte order 16-bit integer */
   uint16_t util_letoh16(uint16_t le16);
   
   /** Convert 32-bit integer from host byte order to big endian byte order
    * @param h32 Host byte order 32-bit integer */
   uint32_t util_htobe32(uint32_t h32);
   
   /** Convert 32-bit integer from little endian byte order to host byte order
    * @param h32 Host byte order 32-bit integer */
   uint32_t util_htole32(uint32_t h32);
   
   /** Convert 32-bit integer from big endian byte order to host byte order
    * @param be32 Big endian byte order 32-bit integer */
   uint32_t util_betoh32(uint32_t be32);
   
   /** Convert 32-bit integer from little endian byte order to host byte order
    * @param le32 Little endian byte order 32-bit integer */
   uint32_t util_letoh32(uint32_t le32);
   
   /** Convert 64-bit integer from host byte order to big endian byte order
    * @param h64 Host byte order 64-bit integer */
   uint64_t util_htobe64(uint64_t h64);
   
   /** Convert 64-bit integer from host byte order to little endian byte order
    * @param h64 Host byte order 64-bit integer */
   uint64_t util_htole64(uint64_t h64);
   
   /** Convert 64-bit integer from big endian byte order to host byte order
    * @param be64 Big endian byte order 64-bit integer */
   uint64_t util_betoh64(uint64_t be64);
   
   /** Convert 64-bit integer from little endian byte order to host byte order
    * @param le64 Little endian byte order 64-bit integer */
   uint64_t util_letoh64(uint64_t le64);

Print and print formatting functions
------------------------------------

Time
^^^^

The time module contains function for formatting a timestamp:

.. code-block:: c

   int strtime(char *str, int64_t utime, int year_base);

The formatted timestamp will be returned in ``str``, ``utime`` is the timestamp as a 64 bit signed integer representing milliseconds.
 
If the ``year_base`` is 1970, the formatted timestamp returned in ``str`` will be:

``day-month-year hour:minute:second.millisecond``, 

Example ``22-06-2015 12:34:56.234``.

If the ``year_base`` is not 1970, the formatted timestamp returned in ``str`` will treated as delta time 
and will just count the years, months, days, hours, minutes, seconds and milliseconds. If the number years is larger than zero,
the formatted string will be:
 
``<years> years <months> months <days> days <hours> hours <minutes> minutes <seconds> seconds <milliseconds> milliseconds``

For each counter (years, months, days, ...), while the value is zero, the value will be omitted in the output, 
e.g. if years and months are zero, the format will be:

``<days> days <hours> hours <minutes> minutes <seconds> seconds <milliseconds> milliseconds``

If the ``utime`` parameter is negative, the formatted string will be prefixed with a '-'.

Byte size
^^^^^^^^^

Format a string with a byte size in the format <size> M/K/B, counting the number of 1048576/1024/1 byte blocks respectively.

.. code-block:: c

   void bytesize(char *buf, int len, long n);

Example:

.. code-block:: c

   char my_str[128];
   
   bytesize(my_str, 128, 512);
   bytesize(my_str, 128, 1024);
   bytesize(my_str, 128, 1048576);

will return the result ``512.0B``, ``1.0K`` and ``1.0M`` in ``my_str``.


Color printf
^^^^^^^^^^^^

Color printf allows printing string to the console with colors, using the standard terminal escape sequences for setting the color.

.. code-block:: c

   void color_printf(color_printf_t color_arg, const char * format, ...);

The following colors are defined:

.. code-block:: c

   COLOR_NONE
   COLOR_BLACK
   COLOR_RED
   COLOR_GREEN
   COLOR_YELLOW
   COLOR_BLUE
   COLOR_MAGENTA
   COLOR_CYAN
   COLOR_WHITE
   
The following colors attributes are defined:

.. code-block:: c

   COLOR_BOLD

The colors and color attributes must be OR'ed together, e.g. to print a string in green bold:

.. code-block:: c

   color_printf(COLOR_GREEN | COLOR_BOLD, "Hello bold-green-world\r\n");
 
Base16
^^^^^^

The base16 module contains functions for encoding and decoding base16 arrays to and from strings.

.. code-block:: c

   /** Base16-encode data
    *
    * The buffer must be the correct length for the encoded string.  Use
    * something like
    *
    *     char buf[ base16_encoded_len ( len ) + 1 ];
    *
    * (the +1 is for the terminating NUL) to provide a buffer of the
    * correct size.
    *
    * @param raw Raw data
    * @param len Length of raw data
    * @param encoded Buffer for encoded string */
   void base16_encode(uint8_t *raw, size_t len, char *encoded);
   
   /** Base16-decode data
    *
    * The buffer must be large enough to contain the decoded data.  Use
    * something like
    *
    *     char buf[ base16_decoded_max_len ( encoded ) ];
    *
    * to provide a buffer of the correct size.
    * @param encoded Encoded string
    * @param raw Raw data
    * @return Length of raw data, or negative error */
   int base16_decode(const char *encoded, uint8_t *raw);

The base16 module also contains two utility functions for calculating required length of buffers for encoding and decoding:

.. code-block:: c

   /** Calculate length of base16-encoded data
    * @param raw_len Raw data length
    * @return Encoded string length (excluding NUL) */
   static inline size_t base16_encoded_len(size_t raw_len) {
      return (2 * raw_len);
   }
   
   /** Calculate maximum length of base16-decoded string
    * @param encoded Encoded string
    * @return Maximum length of raw data */
   static inline size_t base16_decoded_max_len(const char *encoded) {
      return ((strlen(encoded) + 1) / 2);
   }



Hexdump
^^^^^^^
The hexdump module contains a function for dumping a piece of memory as hex numbers and ascii characters:

.. code-block:: c

   void hex_dump(void *src, int len);

Example:

.. code-block:: c

   char buf[32];
   
   memset(buf, 0, 32);
   strcpy(buf, "01234567");
   hex_dump(buf, 32);

will print the following:

.. code-block:: c

   80010120 : 30 31 32 33 34 35 36 37  00 00 00 00 00 00 00 00 | 01234567........
   80010130 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................

where ``80010120`` is the address of ``buf``. The dots to the right are printed if the corresponding byte does not respresent a printable character.   
   
Error
^^^^^

The error module contains error code definitions and a function for getting the error code description as a string.

.. code-block:: c

   char *error_string(int code);
   
where ``code`` is one of:

.. code-block:: c

   #define E_NO_ERR -1
   #define E_NO_DEVICE -2
   #define E_MALLOC_FAIL -3
   #define E_THREAD_FAIL -4
   #define E_NO_QUEUE -5
   #define E_INVALID_BUF_SIZE -6
   #define E_INVALID_PARAM -7
   #define E_NO_SS -8
   #define E_GARBLED_BUFFER -9
   #define E_FLASH_ERROR -10
   #define E_BOOT_SER -13
   #define E_BOOT_DEBUG -14
   #define E_BOOT_FLASH -15
   #define E_TIMEOUT -16
   #define E_NO_BUFFER -17
   #define E_OUT_OF_MEM -18
   #define E_FAIL -19

CRC and checksum functions
--------------------------

Crc32
^^^^^

The crc32 module functions for calculating CRC32 from a sequence of bytes.

.. code-block:: c

   /** Calculate single step of crc32 */
   uint32_t chksum_crc32_step(uint32_t crc, uint8_t byte);
   
   /** Caluclate crc32 of a block */
   uint32_t chksum_crc32(uint8_t *block, unsigned int length);

The function ``chksum_crc32`` takes a pointer to a data ``block`` together with a ``length`` parameter 
and then calculates the CRC32 from the data in ``block``.

Normally the function ``chksum_crc32_step`` is not used outside the crc32 module, but it is possible to use
it to manually do the CRC32 calculation. ``chksum_crc32_step`` takes the current CRC32 value ``crc`` and updates it with 
from the ``byte`` parameter.
 
Delay functions
---------------

Delay
^^^^^

The delay module contains a function for waiting (delaying execution) a number of micro seconds.

.. code-block:: c

   /** Init delay system
    * @param freq CPU frequency in Hz */
   void delay_init(uint32_t freq);
   
   /** Delay for number of microseconds
    * @param us Number of microseconds to wait */
   void delay_us(uint32_t us);
   
   /** Delay until condition is true or timeout_us microseconds have passed.
    * @return 0 if condition was met before the timeout, -1 otherwise */
   #define delay_until(condition, timeout_us)
   
Before using the delay functions, the ``delay_init`` should be called with the cpu frequency as parameter,
e.g. ``delay_init(16000000)`` to initialise the delay system with CPU frquency 16 MHz.

The function ``delay_us`` is called with the number of micro seconds to wait, e.g. ``delay_us(1000)`` to wait 1 millisecond.

The macro ``delay_until`` is called with a condition and the number of micro seconds to wait, e.g. ``delay_until(my_condition, 10000)`` 
to wait until ``my_condition`` is not zero or 10 milliseconds have elapsed.

Compression & decompression functions (LZO)
-------------------------------------------

LZO
^^^

The LZO module contains function for compressing and decompressing data using the LZO algorithm.

Functions for compressing and decompressing data:

.. code-block:: c

   int lzo_compress_buffer(void * src, uint32_t src_len, void *out, 
                           uint32_t *out_len, uint32_t out_size);
   int lzo_decompress_buffer(uint8_t * src, uint32_t srclen, uint8_t * dst, 
                             uint32_t dstlen, uint32_t * decomp_len_p);

Functions for reading and writing LZO header:

.. code-block:: c

   int lzo_write_header(lzo_header_t * head);
   
The function ``lzo_write_header`` will fill out a LZO header defined by ``lzo_header_t``, including the header checksum:

.. code-block:: c

   typedef struct __attribute__((packed)) {
      uint8_t magic[9];
      uint16_t version;
      uint16_t libversion;
      uint16_t versionneeded;
      uint8_t method;
      uint8_t level;
      uint32_t flags;
      uint32_t mode;
      uint32_t mtime_low;
      uint32_t mtime_high;
      uint8_t fnamelen;
      uint32_t checksum_header;
      uint8_t data[];
   } lzo_header_t;

Example code for compressing data from memory to memory

.. code-block:: c

   int compress_data(uint8_t *data, uint32_t length, uint8_t *comp, uint32_t max_len) {

      if (length > max_len) {
         log_error("Error compressing data - length too large");
         return -1;
      }
      /* Zip / compress */
      uint32_t comp_len = 0;
      if (lzo_compress_buffer(data, length, comp, &comp_len, max_len) != 0) {
         log_error("Error compressing data");
         return -1;
      }
      
      return comp_len;
   }

Example code for decompressing data from memory to memory

.. code-block:: c

   int decompress_data(uint8_t *data, uint32_t length, uint8_t *decomp, uint32_t max_len) {

      if (length > max_len) {
         log_error("Error decompressing data - length too large");
         return -1;
      }
      /* Unip / decompress */
      uint32_t decomp_len = 0;
      if (lzo_decompress_buffer(data, length, decomp, max_len, &decomp_len) != 0) {
         log_error("Error decompressing data");
         return -1;
      }
      
      return decomp_len;
   }


.. _libutil_uthash:

Hash, List, Linked List, and Array operation
--------------------------------------------
The uthash module is a set of header files containing some rather clever macros. These
macros include **uthash.h** for hash tables, **utlist.h** for linked lists and **utarray.h** for arrays.

The list macros support the basic linked list operations: adding and deleting elements, sorting them and
iterating over them. It does so for both single linked list double linked lists and circular lists.

The dynamic array macros supports basic operations such as push, pop, and erase on the array elements.
These array elements can be any simple datatype or structure. The array operations are based loosely 
on the C++ STL vector methods. Internally the dynamic array contains a contiguous memory region into 
which the elements are copied. This buffer is grown as needed using realloc to accomodate all the data 
that is pushed into it.

The hash tables provides a good alternative to linked lists for larger tables where scanning through 
the entire list is going to be slow. The overhead added is larger memory usage and the additional 
hash processing time, so for short sets of data linked lists are preferred.

Linux specific functions
------------------------
The folder in src/linux contains different helper functions for linux like exporting and setting gpio
pins.

Example code for setting a GPIO pin:

.. code-block:: c
   
   #include <util/gpio.h>
   
   /* Valid GPIOs */
   enum example_gpio {
      GPIO_EXAMPLE_PIN_1,
      GPIO_EXAMPLE_PIN_2,
      GPIO_EXAMPLE_PIN_3,
   };

   static struct sysfs_gpio gpios[] = {
      [GPIO_EXAMPLE_PIN_1]  = { "1", "/sys/class/gpio/gpio1/direction", "/sys/class/gpio/gpio1/value", false },
      [GPIO_EXAMPLE_PIN_2]  = { "2", "/sys/class/gpio/gpio2/direction", "/sys/class/gpio/gpio2/value", false },
      [GPIO_EXAMPLE_PIN_3]  = { "3", "/sys/class/gpio/gpio3/direction", "/sys/class/gpio/gpio3/value", false },
   };

   /* Setting GPIO example pin 2 to high */
   gpio_set(&gpios[GPIO_EXAMPLE_PIN_2], 1)
