template <typename T>
void CC_VK_map_cpy_unmap(VkDevice* logicalDevice, T* src, VkDeviceMemory* devMem, VkBufferCreateInfo* bufInfo) {
	void* data;
	vkMapMemory(*logicalDevice, *devMem, 0, bufInfo->size, 0, &data);
	memcpy(data, src, (size_t)bufInfo->size);
	vkUnmapMemory(*logicalDevice, *devMem);
}