#pragma once

namespace utils
{

template<class _DataType>
ObjData<_DataType>::~ObjData()
{
}

template<class _DataType>
void ObjData<_DataType>::append(std::string CONST_REFERENCE name, _DataType CONST_PTR data_ptr, size_t size)
{
	CVK_ASSERT(data_ptr != nullptr);
	CVK_ASSERT(size > 0);

	auto iter = _data.find(name);
	if (iter == _data.end()) {
		_data.insert(std::pair<KeyType, ValType>(name, {}));
	}
	ValType& data = _data[name];
	size_t old_size = data.size();
	data.resize(old_size + size);
	memcpy(&data[old_size], data_ptr, sizeof(_DataType) * size);

}
template<class _DataType>
void ObjData<_DataType>::append(std::string CONST_REFERENCE name, _DataType CONST_REFERENCE data_ptr)
{
	append(name, &data_ptr, 1);
}

template<class _DataType>
auto ObjData<_DataType>::operator[](std::string CONST_REFERENCE name) const -> std::vector<_DataType> CONST_REFERENCE
{
	return _data[name];
}
template<class _DataType>
auto ObjData<_DataType>::operator[](std::string CONST_REFERENCE name) -> std::vector<_DataType>&
{
	return _data[name];
}

} // namespace utils
