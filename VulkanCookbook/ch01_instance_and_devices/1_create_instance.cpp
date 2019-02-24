/*
	@Note		
	Enable instance layer / extension
		instance layer 지원 여부 체크
		instance extension 지원 여부 체크
	Create instance
		어플리케이션 정보 설정
		instance layer/extension 정보 설정
*/

//instance layer 지원 여부 체크	
//instance extension 지원 여부 체크	
bool CheckAvailableInstanceExtensions( std::vector<VkExtensionProperties> & available_extensions ) {
	uint32_t extensions_count = 0;
	VkResult result = VK_SUCCESS;

	result = vkEnumerateInstanceExtensionProperties( nullptr, &extensions_count, nullptr );
	if( (result != VK_SUCCESS) || (extensions_count == 0) ) {
		std::cout << "Could not get the number of instance extensions." << std::endl;
		return false;
	}

	available_extensions.resize( extensions_count );
	result = vkEnumerateInstanceExtensionProperties( nullptr, &extensions_count, available_extensions.data() );
	if( (result != VK_SUCCESS) ||(extensions_count == 0) ) {
	  std::cout << "Could not enumerate instance extensions." << std::endl;
	  return false;
	}

	return true;
}

bool IsExtensionSupported( std::vector<VkExtensionProperties> const & available_extensions,
                             char const * const extension ) {
	for( auto & available_extension : available_extensions ) {
	  if( strstr( available_extension.extensionName, extension ) ) {
		return true;
	  }
	}
	return false;
}

//---------------------------------------------------------------------------------
/* Create instance */

bool CreateVulkanInstance( std::vector<char const *> const & desired_extensions,
						 char const * const                application_name,
						 VkInstance                      & instance ) {
	std::vector<VkExtensionProperties> available_extensions;
	if( !CheckAvailableInstanceExtensions( available_extensions ) ) {
	  return false;
	}

	for( auto & extension : desired_extensions ) {
	  if( !IsExtensionSupported( available_extensions, extension ) ) {
		std::cout << "Extension named '" << extension << "' is not supported by an Instance object." << std::endl;
		return false;
	  }
	}

	//어플리케이션 정보 설정
	VkApplicationInfo application_info = {
	  VK_STRUCTURE_TYPE_APPLICATION_INFO,                 // VkStructureType           sType
	  nullptr,                                            // const void              * pNext
	  application_name,                                   // const char              * pApplicationName
	  VK_MAKE_VERSION( 1, 0, 0 ),                         // uint32_t                  applicationVersion
	  "Vulkan Cookbook",                                  // const char              * pEngineName
	  VK_MAKE_VERSION( 1, 0, 0 ),                         // uint32_t                  engineVersion
	  VK_MAKE_VERSION( 1, 0, 0 )                          // uint32_t                  apiVersion
	};
	
	//instance layer/extension 정보 설정
	VkInstanceCreateInfo instance_create_info = {
	  VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,             // VkStructureType           sType
	  nullptr,                                            // const void              * pNext
	  0,                                                  // VkInstanceCreateFlags     flags
	  &application_info,                                  // const VkApplicationInfo * pApplicationInfo
	  0,                                                  // uint32_t                  enabledLayerCount
	  nullptr,                                            // const char * const      * ppEnabledLayerNames
	  static_cast<uint32_t>(desired_extensions.size()),   // uint32_t                  enabledExtensionCount
	  desired_extensions.data()                           // const char * const      * ppEnabledExtensionNames
	};

	VkResult result = vkCreateInstance( &instance_create_info, nullptr, &instance );
	if( (result != VK_SUCCESS) ||
		(instance == VK_NULL_HANDLE) ) {
	  std::cout << "Could not create Vulkan instance." << std::endl;
	  return false;
	}

return true;
}


	
	