#pragma once
#include "object.h"
//#////////////////////////////#//
//////#//objekty_swiata//#////////
//#////////////////////////////#//

//////#//stdrt_obj//#////////
void stdrt_object::set_pos(XMVECTOR _pos) {
	pos = _pos;
}
XMVECTOR stdrt_object::get_pos() {
	return pos;
}
void stdrt_object::set_kier(XMVECTOR _kier) {
	kier = _kier;
}
XMVECTOR stdrt_object::get_kier() {
	return kier;
}
std::string stdrt_object::get_opis() {
	return opis;
}
float stdrt_object::get_size() {
	return size;
}
void stdrt_object::set_size(float _size) {
	size = _size;
}