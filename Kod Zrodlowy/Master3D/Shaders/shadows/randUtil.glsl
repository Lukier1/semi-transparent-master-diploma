#version 420

float lastState = 1;

uint TausStep(uint z, uint S1, uint S2, uint S3, uint M) {
	uint b = (((z << S1) ^ z) >> S2);
	return (((z & M) << S3) ^ b);
}

uint LCGStep(uint z, uint A, uint C)
{
  return A*z+C;
}


uint z1, z2, z3, z4;

void setSeed(vec3 seed) {
	vec3 bigSeed = mod(456789*seed, 1013904223);
	z1 = uint(bigSeed.x);
	z2 = uint(bigSeed.y);
	z3 = uint(bigSeed.z);
	z4 = z3^z2^z1;
}

float HybridTaus() {
	z1 = TausStep(z1, 13, 19, 12, 4294967294);
	z2 = TausStep(z2, 2, 25, 4, 4294967288);
	z3 = TausStep(z3, 3, 11, 17, 4294967280);
	z4 = LCGStep(z4, 1664525, 1013904223);

	return 2.3283064365387e-10 * float(
		z1 ^    // p1=2^31-1
		z2 ^   // p2=2^30-1
		z3 ^   // p3=2^28-1
		z4     // p4=2^32
	);
}

float nextRandFloat() {
	return HybridTaus();
}