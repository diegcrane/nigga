auto sdk::resolve_api_set(sdk::__multibyte_string name) -> std::vector<sdk::__unicode_string>
{
	auto module_names = std::vector<sdk::__unicode_string>();

	if (!(name.left_find(XOR("api-")) == 0 || name.left_find(XOR("ext-")) == 0))
		return module_names;

	if (name.right_find(XOR(".dll")) != (unsigned __int64)-1)
		name.right_pop(4);

	auto process_enviroment_block = sdk::get_process_enviroment_block();

	auto api_set_map = (_API_SET_NAMESPACE*)process_enviroment_block->Reserved9[0];

	auto api_set_namespace_entry = (_API_SET_NAMESPACE_ENTRY*)((unsigned __int64)api_set_map + api_set_map->EntryOffset);

	for (unsigned __int32 x = 0; x < api_set_map->Count && module_names.size() == 0; ++x, ++api_set_namespace_entry)
	{
		if (sdk::__unicode_string((wchar_t*)((unsigned __int64)api_set_map + api_set_namespace_entry->NameOffset)).left_find(name.get_unicode_string()) == (unsigned __int64)-1)
			continue;

		auto api_set_value_entry = (_API_SET_VALUE_ENTRY*)((unsigned __int64)api_set_map + api_set_namespace_entry->ValueOffset);

		for (unsigned __int32 y = 0; y < api_set_namespace_entry->ValueCount; ++y, ++api_set_value_entry)
			module_names.push_back(sdk::__unicode_string((wchar_t*)((unsigned __int64)api_set_map + api_set_value_entry->ValueOffset), api_set_value_entry->ValueLength / 2));
	}

	if (module_names.size() == 0 && name.get_length() != 0)	//why the fuck are import descriptors pointing to api set versions that are not even loaded? pop till we find the closest match?
	{
		name.right_pop();

		module_names = sdk::resolve_api_set(name);
	}

	//if the passed name was a valid api set and we still found no module name we should end it all cos honestly im not loading a module just to resolve its fucking name fuck microsoft fuck satya nadella

	return module_names;
}
