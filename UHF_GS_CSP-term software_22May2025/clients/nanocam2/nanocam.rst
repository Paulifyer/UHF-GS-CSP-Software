**********
Client API
**********

The client API consists of a set of wrapper functions that simplify the CSP interface to the NanoCam C1U. These functions are implemented in the ``nanocam.c`` file and can be integrated in custom code by including the ``nanocam.h`` header file. The ``cmd_nanocam.c`` implements the GOSH commands for the NanoCam and can be used as an additional reference for the use of the client API.

All the client functions specify a ``timeout`` argument that is used to specify the maximum number of milliseconds to wait for a reply. The client interface automatically performs endian conversion to network byte order on all arguments.

The functions return 0 on success and a non-zero error code on error. The error codes are listed below:

.. code-block:: c

  #define NANOCAM_ERROR_NONE		0 /** No error */
  #define NANOCAM_ERROR_NOMEM		1 /** Not enough space */
  #define NANOCAM_ERROR_INVAL		2 /** Invalid value */
  #define NANOCAM_ERROR_IOERR		3 /** I/O error */
  #define NANOCAM_ERROR_NOENT		4 /** No such file or directory */

Similar to the GOSH interface, the client API operates on a single C1U at a time. The CSP address of this C1U is set using the ``nanocam_set_node`` function. By default, the commands operate on CSP node ``NANOCAM_DEFAULT_ADDRESS`` which is currently set to 6. If the camera address has not been changed, it is not necessary to call ``nanocam_set_node``.

.. c:function:: void nanocam_set_node(uint8_t node)

  This function sets the CSP address of the NanoCam C1U. All other API functions use this CSP address.

Image Capture
=============

Capture of images is provided by the ``nanocam_snap`` function.

.. c:function:: int nanocam_snap(cam_snap_t *snap, cam_snap_reply_t *reply, unsigned int timeout)

  This function is used to capture an image. The capture parameters should be set in the ``snap`` structure argument prior to calling this function. The reply from the camera is returned in the ``reply`` struct. 

----------

.. c:type:: cam_snap_t

  The ``cam_snap_t`` struct is used to specify the arguments for the snap command. Each field of the structure is documented below.

.. c:member:: uint32_t cam_snap_t.flags

  This argument supplies optional flag bits that modifies the behavior of the snap request. 

.. code-block:: c

  #define NANOCAM_SNAP_FLAG_AUTO_GAIN	(1 <<  0) /** Automatically adjust gain */
  #define NANOCAM_SNAP_FLAG_STORE_RAM	(1 <<  8) /** Store snapped image to RAM */
  #define NANOCAM_SNAP_FLAG_STORE_FLASH	(1 <<  9) /** Store snapped image to flash */
  #define NANOCAM_SNAP_FLAG_STORE_THUMB	(1 << 10) /** Store thumbnail to flash */
  #define NANOCAM_SNAP_FLAG_STORE_TAGS	(1 << 11) /** Store image tag file */
  #define NANOCAM_SNAP_FLAG_NOHIST	(1 << 16) /** Do not calculate histogram */
  #define NANOCAM_SNAP_FLAG_NOEXPOSURE	(1 << 17) /** Do not adjust exposure */

.. c:member:: uint8_t cam_snap_t.count

  Number of images to capture. If set to zero a single image capture will be performed. When capturing multiple images, the ``nanocam_snap`` function will only return the ``cam_snap_reply_t`` for the first image.

.. c:member:: uint8_t cam_snap_t.format

  Output format to use when ``NANOCAM_SNAP_FLAG_STORE_FLASH`` or ``NANOCAM_SNAP_FLAG_STORE_RAM`` is enabled in the flags field. Valid output formats are:

.. code-block:: c

  #define NANOCAM_STORE_RAW		0 /* Store RAW sensor output */
  #define NANOCAM_STORE_BMP		1 /* Store bitmap output */
  #define NANOCAM_STORE_JPG		2 /* Store JPEG compressed output */
  #define NANOCAM_STORE_DNG		3 /* Store DNG output (Raw, digital negative) */

.. c:member:: uint16_t cam_snap_t.delay

  Optional delay between captures in milliseconds. Only applicable when count > 1.

.. c:member:: uint16_t cam_snap_t.width

  Image width in pixels. Set to 0 to use default (maximum = 2048) size.

.. c:member:: uint16_t cam_snap_t.height

  Image height in pixels. Set to 0 to use default (maximum = 1536) size.

.. c:member:: uint16_t cam_snap_t.top

  Image crop rectangle top coordinate. Must be set to 0.

.. c:member:: uint16_t cam_snap_t.left

  Image crop rectangle left coordinate. Must be set to 0.

----------

.. c:type:: cam_snap_reply_t

  This struct contains the reply of a image capture. The reply contains arrays with information of average brightness and distribution. A couple of defines are used for the length of these arrays:

.. code-block:: c

  #define NANOCAM_SNAP_COLORS		4
  #define NANOCAM_SNAP_HIST_BINS	16

.. c:member:: uint8_t cam_snap_reply_t.result

  Result of the capture. One of the error codes listed in the introduction.

.. c:member:: uint8_t cam_snap_reply_t.seq

  Zero-index sequence number when capturing multiple images, i.e. when ``count`` > 1 in the ``cam_snap_t`` argument. 
  
.. c:member:: uint8_t[NANOCAM_SNAP_COLORS] cam_snap_reply_t.light_avg

  Array of ``NANOCAM_SNAP_COLORS`` elements corresponding to the average brightness of all pixels plus the red, green and blue channel pixels. The numbers are scaled from 0-255, so e.g. 128 corresponds to an average brightness of 50%. 

.. c:member:: uint8_t[NANOCAM_SNAP_COLORS] cam_snap_reply_t.light_peak

  Array of ``NANOCAM_SNAP_COLORS`` elements corresponding to the estimated peak brightness of all pixels plus the red, green and blue channel pixels.

.. c:member:: uint8_t[NANOCAM_SNAP_COLORS] cam_snap_reply_t.light_min

  Array of ``NANOCAM_SNAP_COLORS`` elements corresponding to the minimum brightness of all pixels plus the red, green and blue channel pixels.

.. c:member:: uint8_t[NANOCAM_SNAP_COLORS] cam_snap_reply_t.light_max

  Array of ``NANOCAM_SNAP_COLORS`` elements corresponding to the maximum brightness of all pixels plus the red, green and blue channel pixels.

.. c:member:: uint8_t[NANOCAM_SNAP_COLORS][NANOCAM_SNAP_HIST_BINS] cam_snap_reply_t.hist

  Array of ``NANOCAM_SNAP_COLORS`` elements each consisting on an array of ``NANOCAM_SNAP_HIST_BINS`` bins. Each bin contains a number from 0 to 255 matching the distribution of brightness, with the sum of all bins being 255. Thus, if a bin is 128, 50% of all pixels falls within the brightness range covered by that particular bin.
  
Image Storage
=============

Storage of images captured to the snap buffer is provided by the ``nanocam_store`` functions. Images are stored in the ``/mnt/data/images`` directory on the camera file system.

.. c:function:: int nanocam_store(cam_store_t *store, cam_store_reply_t *reply, unsigned int timeout)

  This function is used to store a captured image from the snap buffer to persistant storage and/or RAM.

----------

.. c:type:: cam_store_t

  This struct is used to supply store arguments to the ``nanocam_store`` function.

.. c:member:: uint8_t cam_store_t.format

  Output format of the stored image. See the argument list to ``cam_snap_t.format`` for a list of valid options.

.. c:member:: uint8_t cam_store_t.scale

  This argument is currently unused and should be set to 0.

.. c:member:: uint32_t cam_store_t.flags

  This argument supplies optional flag bits that modifies the behavior of the store request. If the ``NANOCAM_STORE_FLAG_FREEBUF`` flag is cleared, a copy of the stored image will be kept in the RAM list.

.. code-block:: c

  #define NANOCAM_STORE_FLAG_FREEBUF      (1 << 0) /* Free buffer after store */
  #define NANOCAM_STORE_FLAG_THUMB        (1 << 1) /* Create thumbnail */
  #define NANOCAM_STORE_FLAG_TAG          (1 << 2) /* Create tag file */

.. c:member:: char[40] cam_store_t.filename

  Filename of the stored image. The file type is not required to match the file format, but it is recommend to e.g. store JPEG images with a *.jpg* ending. Setting this field to an empty string, i.e. set filename[0] to ``\0``, will only store the image in the RAM list.

----------

.. c:type:: cam_store_reply_t

  This struct contains the reply of a image store command.

.. c:member:: uint8_t cam_store_reply_t.result

  Result of the store command. One of the error codes listed in the introduction.

.. c:member:: uint32_t cam_store_reply_t.image_ptr

  Address of the RAM copy of the stored image. 

.. c:member:: uint32_t cam_store_reply_t.image_size

  Size in bytes of the RAM copy of the stored image.

Modifying Sensor Registers
==========================

The image sensor registers can be adjusted using the ``nanocam_reg_read`` and ``nanocam_reg_write`` functions. Any modifications of the registers are volatile, and may be overridden by the auto-gain and exposure setting algorithms.

.. note:: For normal operation, it is not necessary to adjust sensor registers directly. Instead the image configuration parameters from table 1 should be used. See more in :doc:`/doc/parameters`.

Please refer to the *Aptina MT9T031* datasheet for a description of individual sensor registers.

.. c:function:: int nanocam_reg_read(uint8_t reg, uint16_t *value, unsigned int timeout)

  This function reads a sensor register and returns the current value. The ``reg`` argument contains the address of the register to read and the current value is returned in the ``value`` pointer.

.. c:function:: int nanocam_reg_write(uint8_t reg, uint16_t value, unsigned int timeout)

  Use this function to update the value of a register. The ``reg`` contains the register address and ``value`` contains the new value to write to the register.

In-memory Images
================

.. c:function:: int nanocam_img_list(nanocam_img_list_cb cb, unsigned int timeout)

  Call this function to list all images in the RAM list. The ``nanocam_img_list_cb`` callback will be called once for each image element in the list. 

.. c:type:: typedef void (*nanocam_img_list_cb)(int seq, cam_list_element_t *elm)

  Implement an image listing callback function matching this prototype, and supply it to the ``nanocam_img_list`` list function. If no images are available in memory, the callback is called with ``elm`` set to NULL.

.. c:function:: int nanocam_img_list_flush(unsigned int timeout)

  This function flushes all images stored in the RAM image list. Note that the current image in the snap buffer can not be flushed, so a single image will always be returned by ``nanocam_img_list``.

Focus Assist Routine
====================

.. c:function:: int nanocam_focus(uint8_t algorithm, uint32_t *af, unsigned int timeout)

  This function runs a single iteration of the focus assist algorithm. The ``algorithm`` argument is used to select between different algorithms. Currently, ``NANOCAM_AF_JPEG`` is the only supported option.

  The focus assist algorithm captures an image, JPEG compresses the center of the image and returns the size of the compressed data in the ``af`` pointer. The premise is that a more focused image will be more difficult to compress, giving a larger size of the compressed data. Continuously running this algorithm can thus be used to adjust the focus until a maximum size is found.

Data Partition Recovery
=======================

.. c:function:: int nanocam_recoverfs(unsigned int timeout)

 This function can be used to recreate the data file system. Note that this erases ALL images stored on the camera. If you just want to delete all captured images, using the FTP ``rm`` command is much faster and a safer option than rebuilding the entire file system. 

