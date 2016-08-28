//struct SSEcosmoType {};
//#define COSMO_CONST(x) x
//int cosmoMask = 1;
//float andnot(SSEcosmoType, SSEcosmoType);
//
//void SPLINEQ(SSEcosmoType invr, SSEcosmoType r2, SSEcosmoType twoh, SSEcosmoType& a, SSEcosmoType& b, SSEcosmoType& c, SSEcosmoType& d) {
//	SSEcosmoType dir = invr;
//	SSEcosmoType select0 = r2 < twoh * twoh;
//	int compare0 = movemask(select0);
//	if (!compare0) {
//		a = dir;
//		b = a * a * a;
//		c = COSMO_CONST(3.0) * b * a * a;
//		d = COSMO_CONST(5.0) * c * a * a;
//	} else {
//		SSEcosmoType a0, b0, c0, d0, a1, b1, c1, d1, a2, b2, c2, d2;
//		SSEcosmoType dih = COSMO_CONST(2.0) / twoh;
//		SSEcosmoType u = dih / dir;
//		if ((~compare0) & cosmoMask) {
//			a0 = dir;
//			b0 = a0 * a0 * a0;
//			c0 = COSMO_CONST(3.0) * b0 * a0 * a0;
//			d0 = COSMO_CONST(5.0) * c0 * a0 * a0;
//		}
//		SSEcosmoType select1 = u < COSMO_CONST(1.0);
//		int compare1 = movemask(select1);
//		if (compare1) {
//			a1 = dih;
//			b1 = dih;
//			c1 = dih;
//			d1 = dih;
//		}
//		if ((~compare1) & cosmoMask) {
//			a2 = dih;
//			b2 = dih;
//			c2 = dih;
//			d2 = dih;
//		}
//		a = andnot(select0, a0) | (select0 & ((select1 & a1) | andnot(select1, a2)));
//		b = andnot(select0, b0) | (select0 & ((select1 & b1) | andnot(select1, b2)));
//		c = andnot(select0, c0) | (select0 & ((select1 & c1) | andnot(select1, c2)));
//		d = andnot(select0, d0) | (select0 & ((select1 & d1) | andnot(select1, d2)));
//	}
//}
