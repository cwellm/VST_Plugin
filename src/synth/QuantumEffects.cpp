#include "QuantumEffects.h"

namespace cw::synth {

cVector Spin3Rotation::matMult(cMatrix matrix, cVector inVec, float factor = 1) {
	cVector result;
	int dimension = inVec.size();
	for (int row = 0; row < dimension; ++row) {
		result.push_back(0);
	}
	for (int row = 0; row < dimension; ++row) {
		for (int col = 0; col < dimension; ++col) {
			result.at(row) += matrix.at(row).at(col) * inVec.at(col) * factor;
		}
	}

	return result;
}

cVector Spin3Rotation::cVecAdd(cVector v0, cVector v1) {
	cVector result;
	auto it0 = v0.begin();
	auto it1 = v1.begin();
	while (it0 != v0.end()) {
		result.push_back(*it0 + *it1);
		++it0;
	}
	return result;
}

std::array<std::vector<float>, 2> Spin3Rotation::spinRotate(std::array<std::vector<float>, 2> inVec) {

	
	cVector inCVec;
	int inVecSize = inVec[0].size();
	int bufSize = buffer.size();
	// TODO: make it safe: Up to now, it is expected that always a vector of sufficient size comes in. The check now does
	// not make so much sense...but it is at least safe
	if (inVecSize < bufSize + 4) {
		return inVec;
	}

	// First, check the buffer whether it has remaining elements and pick those. 
	while (!buffer.empty()) {
		inCVec.push_back(buffer.front());
		buffer.pop();
	}
	// Construct the input complex vector: Real and imag part from left and right channel. If the input vector together 
	// with the previous buffer size is not a multiple of 4, push the remainder into the buffer. This is necessary, as 
	// in the following, we will need vectors which have size 4.
	int totSize = inCVec.size() + inVecSize;
	int remain = totSize % 4;

	for (int i = 0; i< inVec[0].size() - remain; ++i) {
		inCVec.push_back(std::complex<float>(inVec[0].at(i), inVec[1].at(i)));
	}
	for (int i = inVecSize - remain; i < inVecSize; ++i) {
		buffer.push(std::complex<float>(inVec[0].at(i), inVec[1].at(i)));
	}

	
	// Now, do the transformation - from here, I know now that the inCVec will have a length of multiples of 4.
	std::vector<float> resultR;
	std::vector<float> resultL;
	for (int i = 0; i < inCVec.size() - 3; i += 4) {
		cVector chunk;
		for (int j = 0; j < 4; ++j) {
			chunk.push_back(inCVec.at(i + j));
		}

		auto outX = matMult(spins.S_x, chunk, std::cos(phi) * std::sin(theta));
		auto outY = matMult(spins.S_y, chunk, std::sin(phi) * std::sin(theta));
		auto outZ = matMult(spins.S_z, chunk, std::cos(theta));
		auto outTotal = cVecAdd(outX, cVecAdd(outY, outZ));

		for (int k = 0; k < 4; ++k) {
			resultL.push_back(outTotal.at(k).real());
			resultR.push_back(outTotal.at(k).imag());
		}
	}
	
	return std::array<std::vector<float>, 2>{resultL, resultR};
	// TODO: loudness scaling
}

void Spin3Rotation::clearBuffer() {
	while (!buffer.empty()) {
		buffer.pop();
	}
}

} // namespace cw::synth