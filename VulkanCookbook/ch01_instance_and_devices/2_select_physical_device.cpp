/* 
	@Note
	시스템에서 사용가능한 GPU 열거
	queue family 지원 여부 확인
		- GPU 속송에서 지원하는 queue family확인
	GPU 프로퍼티 / 피처 확인
*/

//시스템에서 사용가능한 GPU 열거
bool EnumerateAvailablePhysicalDevices( VkInstance instance,
									  std::vector<VkPhysicalDevice> & available_devices ) {
	uint32_t devices_count = 0;
	VkResult result = VK_SUCCESS;

	result = vkEnumeratePhysicalDevices( instance, &devices_count, nullptr );
	if( (result != VK_SUCCESS) ||
		(devices_count == 0) ) {
	  std::cout << "Could not get the number of available physical devices." << std::endl;
	  return false;
	}

	available_devices.resize( devices_count );
	result = vkEnumeratePhysicalDevices( instance, &devices_count, available_devices.data() );
	if( (result != VK_SUCCESS) ||
		(devices_count == 0) ) {
	  std::cout << "Could not enumerate physical devices." << std::endl;
	  return false;
	}

	return true;
}

//queue family 지원 여부 확인
std::vector<VkPhysicalDevice> physical_devices;
EnumerateAvailablePhysicalDevices( *Instance, physical_devices );

for( auto & physical_device : physical_devices ) {
  if( !SelectIndexOfQueueFamilyWithDesiredCapabilities( physical_device, VK_QUEUE_GRAPHICS_BIT, GraphicsQueue.FamilyIndex ) ) {
	continue;
  }

  if( !SelectIndexOfQueueFamilyWithDesiredCapabilities( physical_device, VK_QUEUE_COMPUTE_BIT, ComputeQueue.FamilyIndex ) ) {
	continue;
  }

  if( !SelectQueueFamilyThatSupportsPresentationToGivenSurface( physical_device, *PresentationSurface, PresentQueue.FamilyIndex ) ) {
	continue;
  }
  
  ...
}

//- GPU 속송에서 지원하는 queue family 확인
bool SelectQueueFamilyThatSupportsPresentationToGivenSurface( VkPhysicalDevice   physical_device,
															VkSurfaceKHR       presentation_surface,
															uint32_t         & queue_family_index ) {
	std::vector<VkQueueFamilyProperties> queue_families;
	if( !CheckAvailableQueueFamiliesAndTheirProperties( physical_device, queue_families ) ) {
	  return false;
	}

	for( uint32_t index = 0; index < static_cast<uint32_t>(queue_families.size()); ++index ) {
	  VkBool32 presentation_supported = VK_FALSE;
	  VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR( physical_device, index, presentation_surface, &presentation_supported );
	  if( (VK_SUCCESS == result) &&
		  (VK_TRUE == presentation_supported) ) {
		queue_family_index = index;
		return true;
	  }
	}
	return false;
}

bool CheckAvailableQueueFamiliesAndTheirProperties( VkPhysicalDevice physical_device,
												  std::vector<VkQueueFamilyProperties> & queue_families ) {
	uint32_t queue_families_count = 0;

	vkGetPhysicalDeviceQueueFamilyProperties( physical_device, &queue_families_count, nullptr );
	if( queue_families_count == 0 ) {
	  std::cout << "Could not get the number of queue families." << std::endl;
	  return false;
	}

	queue_families.resize( queue_families_count );
	vkGetPhysicalDeviceQueueFamilyProperties( physical_device, &queue_families_count, queue_families.data() );
	if( queue_families_count == 0 ) {
	  std::cout << "Could not acquire properties of queue families." << std::endl;
	  return false;
	}

	return true;
}
