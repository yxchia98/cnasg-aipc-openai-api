// Copyright John Maddock 2019.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SC_
#  define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
#endif
   static const std::array<std::array<T, 4>, 12> hypergeometric_1F1_big = {{
      { SC_(-8.1472375000000000000000000000000000000000e+05), SC_(-1.3586878906250000000000000000000000000000e+04), SC_(-1.5873352050781250000000000000000000000000e+01), SC_(4.9489925464971372677922628162736666342744e-401) }, 
      { SC_(-1.3547703125000000000000000000000000000000e+04), SC_(-1.2525131835937500000000000000000000000000e+03), SC_(-1.5873352050781250000000000000000000000000e+01), SC_(-2.5898494644592200447792014846620256493447e+43) }, 
      { SC_(-9.0579218750000000000000000000000000000000e+03), SC_(-1.2525131835937500000000000000000000000000e+03), SC_(-1.5873352050781250000000000000000000000000e+01), SC_(8.3580772868001229331683618349570363666238e-49) }, 
      { SC_(9.0579179687500000000000000000000000000000e+03), SC_(-1.3586878906250000000000000000000000000000e+04), SC_(-2.7629261207602954767400179815809657975825e-31), SC_(1.0000000000000000000000000000001841950482e+00) }, 
      { SC_(9.0579179687500000000000000000000000000000e+03), SC_(-1.3586878906250000000000000000000000000000e+04), SC_(1.0437607421875000000000000000000000000000e+03), SC_(1.4143132578451013939933694899988696371695e-285) }, 
      { SC_(9.0579179687500000000000000000000000000000e+03), SC_(-1.2525131835937500000000000000000000000000e+03), SC_(1.5873352050781250000000000000000000000000e+01), SC_(2.8816883172369257616825726933718953221443e-48) }, 
      { SC_(1.3547699218750000000000000000000000000000e+04), SC_(-1.3586878906250000000000000000000000000000e+04), SC_(1.0437607421875000000000000000000000000000e+03), SC_(-1.9968249288530089023473568880955473893079e-200) }, 
      { SC_(1.3547699218750000000000000000000000000000e+04), SC_(-1.2525131835937500000000000000000000000000e+03), SC_(1.5873352050781250000000000000000000000000e+01), SC_(-6.9968853248890935672930771162399978395619e+106) }, 
      { SC_(8.1472350000000000000000000000000000000000e+05), SC_(-1.3586878906250000000000000000000000000000e+04), SC_(1.5873352050781250000000000000000000000000e+01), SC_(1.3178378214666009047858314720801604745108e-400) }, 

      // These next 2 should be solvable by A&S 13.3.6 (which does converge nicely for these inputs) but gives the wrong results:

      // Unexpected exception : Error in function boost::math::hypergeometric_pFq<long double> : Cancellation is so severe that no bits in the result are correct, last result was 3.0871891698197084e+73
      { { SC_(-5.9981750131794866e-15), SC_(0.499999999999994), SC_(-240.42092034220695), SC_(1.00000000000004464930530925572237133417488137) }},
      // Unexpected exception : Error in function boost::math::hypergeometric_pFq<long double> : Cancellation is so severe that no bits in the result are correct, last result was 3.0871891698197084e+73
      { { SC_(-5.9981750131794866e-15), SC_(-0.500000000000006), SC_(-240.42092034220695), SC_(1.00000000000003262784934420226963147689063665) }},

      // This one is not too awful, we simply have no better method to improve the error rate:
      // Unexpected high error : 663646.042870225268416106700897216796875 Found : 3.2303512071340211020409327602465054951608181e-07 Expected : 3.23035120665799970299144868238205852151168074e-07
      { { SC_(1.3785990114778025e-10), SC_(-0.99999999986214005), SC_(-3059.6150326728821), SC_(3.23035120665799974283996469721682549854387981e-07) }},

}};
//#undef SC_

