#include "RectF.h"

RectF::RectF() {
	this->left = 0; this->top = 0; this->right = 0; this->bottom = 0;
}

RectF::RectF(float l, float t, float r, float b) {
	this->left = l; this->top = t; this->right = r; this->bottom = b;
}

RectF::~RectF() {

}