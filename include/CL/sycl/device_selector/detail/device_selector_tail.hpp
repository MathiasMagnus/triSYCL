#ifndef TRISYCL_SYCL_DEVICE_SELECTOR_DETAIL_DEVICE_SELECTOR_TAIL_HPP
#define TRISYCL_SYCL_DEVICE_SELECTOR_DETAIL_DEVICE_SELECTOR_TAIL_HPP

/** \file The ending part of of the OpenCL SYCL device_selector

    This is here to break a dependence between device and device_selector

    Ronan at Keryell point FR

    This file is distributed under the University of Illinois Open Source
    License. See LICENSE.TXT for details.
*/

namespace cl {
namespace sycl {

/** \addtogroup execution Platforms, contexts, devices and queues
    @{
*/


/** A device selector by device_type

    \todo To be added to the specification
*/
class device_type_selector : public device_selector {

private:

  /// The device_type to select
  info::device_type device_type;

  /** Cache the default device to select with the default device
      selector.

      This is the host device at construction time and remains as is
      if there is no openCL device */
  device default_device;

public:

  device_type_selector(info::device_type device_type)
    : device_type { device_type } {
    // The default device selection heuristic
    if (device_type == info::device_type::defaults) {
      auto devices = device::get_devices(info::device_type::opencl);
      /* If there is an OpenCL device, pick the first one as the
         default device, other wise it is the host device */
      if (!devices.empty())
        default_device = devices[0];
    }
}

  // To select only the requested device_type
  int operator()(const device &dev) const override {
    if (device_type == info::device_type::all)
      // All devices fit all
      return 1;

    if (device_type == info::device_type::defaults)
      // Only select the default device
      return dev == default_device ? 1 : -1;

    if (device_type == info::device_type::opencl)
      // For now, any non host device is an OpenCL device
      return dev.is_host() ? -1 : 1;

    return dev.type() == device_type ? 1 : -1;
  }

};


/** Select a device by template device_type parameter

    \todo To be added to the specification
*/
template <info::device_type DeviceType>
class device_typename_selector : public device_type_selector {

public:

  device_typename_selector() : device_type_selector { DeviceType } {}

};


/** Devices selected by heuristics of the system

    If no OpenCL device is found then it defaults to the SYCL host device.
*/
using default_selector = device_typename_selector<info::device_type::defaults>;


  /** Select devices according to device type info::device::device_type::gpu
    from all the available OpenCL devices.

    If no OpenCL GPU device is found the selector fails.

    Select the best GPU, if any.
*/
using gpu_selector = device_typename_selector<info::device_type::gpu>;


/** Select devices according to device type info::device::device_type::cpu
    from all the available devices and heuristics

    If no OpenCL CPU device is found the selector fails.
*/
using cpu_selector = device_typename_selector<info::device_type::cpu>;


/** Selects the SYCL host CPU device that does not require an OpenCL
    runtime
*/
using host_selector = device_typename_selector<info::device_type::host>;

/// @} to end the execution Doxygen group

}
}

/*
    # Some Emacs stuff:
    ### Local Variables:
    ### ispell-local-dictionary: "american"
    ### eval: (flyspell-prog-mode)
    ### End:
*/

#endif // TRISYCL_SYCL_DEVICE_SELECTOR_DETAIL_DEVICE_SELECTOR_TAIL_HPP
