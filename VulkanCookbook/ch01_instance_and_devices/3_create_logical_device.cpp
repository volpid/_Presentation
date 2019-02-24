/*
	@Note
	사용 할  queue 명시
	사용 할 디바이스 extension 명시
	사용 할 디바이스 피처 명시
	논리 디바이스 생성
*/

// 디바이스 확장 선택
bool CheckAvailableDeviceExtensions( VkPhysicalDevice physical_device,
								   std::vector<VkExtensionProperties> & available_extensions ) {
	uint32_t extensions_count = 0;
	VkResult result = VK_SUCCESS;

	result = vkEnumerateDeviceExtensionProperties( physical_device, nullptr, &extensions_count, nullptr );
	if( (result != VK_SUCCESS) ||
		(extensions_count == 0) ) {
	  std::cout << "Could not get the number of device extensions." << std::endl;
	  return false;
	}

	available_extensions.resize( extensions_count );
	result = vkEnumerateDeviceExtensionProperties( physical_device, nullptr, &extensions_count, available_extensions.data() );
	if( (result != VK_SUCCESS) ||
		(extensions_count == 0) ) {
	  std::cout << "Could not enumerate device extensions." << std::endl;
	  return false;
	}

	return true;
}

//논리 디바이스 생성
std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

for( auto & info : queue_infos ) {
  queue_create_infos.push_back( {
	VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,       // VkStructureType                  sType
	nullptr,                                          // const void                     * pNext
	0,                                                // VkDeviceQueueCreateFlags         flags
	info.FamilyIndex,                                 // uint32_t                         queueFamilyIndex
	static_cast<uint32_t>(info.Priorities.size()),    // uint32_t                         queueCount
	info.Priorities.data()                            // const float                    * pQueuePriorities
  } );
};

VkDeviceCreateInfo device_create_info = {
  VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,               // VkStructureType                  sType
  nullptr,                                            // const void                     * pNext
  0,                                                  // VkDeviceCreateFlags              flags
  static_cast<uint32_t>(queue_create_infos.size()),   // uint32_t                         queueCreateInfoCount
  queue_create_infos.data(),                          // const VkDeviceQueueCreateInfo  * pQueueCreateInfos
  0,                                                  // uint32_t                         enabledLayerCount
  nullptr,                                            // const char * const             * ppEnabledLayerNames
  static_cast<uint32_t>(desired_extensions.size()),   // uint32_t                         enabledExtensionCount
  desired_extensions.data(),                          // const char * const             * ppEnabledExtensionNames
  desired_features                                    // const VkPhysicalDeviceFeatures * pEnabledFeatures
};

VkResult result = vkCreateDevice( physical_device, &device_create_info, nullptr, &logical_device );
if( (result != VK_SUCCESS) ||
	(logical_device == VK_NULL_HANDLE) ) {
  std::cout << "Could not create logical device." << std::endl;
  return false;
}