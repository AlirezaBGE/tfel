/*!
 * \file   src/UnicodeSupport/UnicodeSupport.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/07/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstring>
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"

namespace tfel {

  namespace unicode {

    const std::vector<UnicodeCharacterDescription>&
    getSupportedUnicodeCharactersDescriptions() {
      static std::vector<UnicodeCharacterDescription> ucds{
          {"\u0391", "tum_0391__", "Alpha (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u0392", "tum_0392__", "Beta (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u0393", "tum_0393__", "Gamma (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u0394", "tum_0394__", "Delta (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u0395", "tum_0395__", "Epsilon (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u0396", "tum_0396__", "Zeta (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u0397", "tum_0397__", "Eta (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u0398", "tum_0398__", "Theta (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u0399", "tum_0399__", "Iota (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u039A", "tum_039A__", "Kappa (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u039B", "tum_039B__", "Lambda (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u039C", "tum_039C__", "Mu (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u039D", "tum_039D__", "Nu (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u039E", "tum_039E__", "Xi (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u039F", "tum_039F__", "Omicron (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u03A0", "tum_03A0__", "Pi (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u03A1", "tum_03A1__", "Rho (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u03A3", "tum_03A3__", "Sigma (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u03A4", "tum_03A4__", "Tau (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u03A5", "tum_03A5__", "Upsilon (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u03A6", "tum_03A6__", "Psi (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u03A7", "tum_03A7__", "Chi (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u03A8", "tum_03A8__", "Psi (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u03A9", "tum_03A9__", "Omega (capital letter)",
           UnicodeCharacterCategory::GREEK_LETTER_CAPITAL},
          {"\u03B1", "tum_03B1__", "Alpha (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03B2", "tum_03B2__", "Beta (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03B3", "tum_03B3__", "Gamma (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03B4", "tum_03B4__", "Delta (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03B5", "tum_03B5__", "Epsilon (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03B6", "tum_03B6__", "Zeta (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03B7", "tum_03B7__", "Eta (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03B8", "tum_03B8__", "Theta (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03B9", "tum_03B9__", "Iota (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03BA", "tum_03BA__", "Kappa (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03BB", "tum_03BB__", "Lambda (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03BC", "tum_03BC__", "Mu (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03BD", "tum_03BD__", "Nu (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03BE", "tum_03BE__", "Xi (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03BF", "tum_03BF__", "Omicron (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03C0", "tum_03C0__", "Pi (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03C1", "tum_03C1__", "Rho (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03C3", "tum_03C3__", "Sigma (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03C4", "tum_03C4__", "Tau (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03C5", "tum_03C5__", "Upsilon (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03C6", "tum_03C6__", "Psi (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03C7", "tum_03C7__", "Chi (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03C8", "tum_03C8__", "Psi (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u03C9", "tum_03C9__", "Omega (small letter)",
           UnicodeCharacterCategory::GREEK_LETTER_SMALL},
          {"\u2215", "tum_2215__", "Division sign",
           UnicodeCharacterCategory::MATHEMATICAL_SYMBOL},
          {"\u2202", "tum_2202__", "Partial differentiation sign",
           UnicodeCharacterCategory::MATHEMATICAL_SYMBOL},
          {"\u2207", "tum_2207__", "Nabla",
           UnicodeCharacterCategory::MATHEMATICAL_SYMBOL},
          {"\u207A", "tum_207A__", "Superscript + sign",
           UnicodeCharacterCategory::MATHEMATICAL_SYMBOL},
          {"\u207B", "tum_207B__", "Superscript - sign",
           UnicodeCharacterCategory::MATHEMATICAL_SYMBOL},
          {"\u208A", "tum_208A__", "Superscript + sign",
           UnicodeCharacterCategory::MATHEMATICAL_SYMBOL},
          {"\u208B", "tum_208B__", "Subscript - sign",
           UnicodeCharacterCategory::MATHEMATICAL_SYMBOL},
          {"\u2297", "tum_2297__", "Tensor product",
           UnicodeCharacterCategory::MATHEMATICAL_OPERATOR},
          {"\u22C5", "tum_22C5__", "Dot sign",
           UnicodeCharacterCategory::MATHEMATICAL_OPERATOR},
          {"\u1D2C", "tum_1D2C__", "A",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D30", "tum_1D30__", "D",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D31", "tum_1D31__", "E",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D33", "tum_1D33__", "G",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D34", "tum_1D34__", "H",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D35", "tum_1D35__", "I",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D36", "tum_1D36__", "J",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D37", "tum_1D37__", "K",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D38", "tum_1D38__", "L",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D39", "tum_1D39__", "M",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D3A", "tum_1D3A__", "N",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D3C", "tum_1D3C__", "O",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D3E", "tum_1D3E__", "P",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D3F", "tum_1D3F__", "R",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D40", "tum_1D40__", "T",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D41", "tum_1D41__", "U",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u2C7D", "tum_2C7D__", "V",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D42", "tum_1D42__", "W",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_CAPITAL},
          {"\u1D43", "tum_1D43__", "a",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D47", "tum_1D47__", "b",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D9C", "tum_1D9C__", "c",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D48", "tum_1D48__", "d",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D49", "tum_1D49__", "e",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1DA0", "tum_1DA0__", "f",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D4D", "tum_1D4D__", "g",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u02B0", "tum_02B0__", "h",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u2071", "tum_2071__", "i",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u02B2", "tum_02B2__", "j",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D4F", "tum_1D4F__", "k",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u02E1", "tum_02E1__", "l",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D50", "tum_1D50__", "m",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u207F", "tum_207F__", "n",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D52", "tum_1D52__", "o",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D56", "tum_1D56__", "p",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u02B3", "tum_02B3__", "r",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u02E2", "tum_02E2__", "s",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D57", "tum_1D57__", "t",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D58", "tum_1D58__", "u",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1D5B", "tum_1D5B__", "v",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u02B7", "tum_02B7__", "w",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u02E3", "tum_02E3__", "x",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u02B8", "tum_02B8__", "y",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u1DBB", "tum_1DBB__", "z",
           UnicodeCharacterCategory::LATIN_LETTER_SUPERSCRIPT_SMALL},
          {"\u2090", "tum_2090__", "a",
           UnicodeCharacterCategory::LATIN_LETTER_SUBSCRIPT_SMALL},
          {"\u2091", "tum_2091__", "e",
           UnicodeCharacterCategory::LATIN_LETTER_SUBSCRIPT_SMALL},
          {"\u2092", "tum_2092__", "o",
           UnicodeCharacterCategory::LATIN_LETTER_SUBSCRIPT_SMALL},
          {"\u2093", "tum_2093__", "x",
           UnicodeCharacterCategory::LATIN_LETTER_SUBSCRIPT_SMALL},
          {"\u2095", "tum_2095__", "h",
           UnicodeCharacterCategory::LATIN_LETTER_SUBSCRIPT_SMALL},
          {"\u2096", "tum_2096__", "k",
           UnicodeCharacterCategory::LATIN_LETTER_SUBSCRIPT_SMALL},
          {"\u2097", "tum_2097__", "l",
           UnicodeCharacterCategory::LATIN_LETTER_SUBSCRIPT_SMALL},
          {"\u2098", "tum_2098__", "m",
           UnicodeCharacterCategory::LATIN_LETTER_SUBSCRIPT_SMALL},
          {"\u209A", "tum_209A__", "p",
           UnicodeCharacterCategory::LATIN_LETTER_SUBSCRIPT_SMALL},
          {"\u209B", "tum_209B__", "s",
           UnicodeCharacterCategory::LATIN_LETTER_SUBSCRIPT_SMALL},
          {"\u209C", "tum_209C__", "t",
           UnicodeCharacterCategory::LATIN_LETTER_SUBSCRIPT_SMALL},
          {"\u1D66", "tum_1D66__", "beta",
           UnicodeCharacterCategory::GREEK_LETTER_SUBSCRIPT_SMALL},
          {"\u1D67", "tum_1D67__", "gamma",
           UnicodeCharacterCategory::GREEK_LETTER_SUBSCRIPT_SMALL},
          {"\u1D68", "tum_1D68__", "rho",
           UnicodeCharacterCategory::GREEK_LETTER_SUBSCRIPT_SMALL},
          {"\u1D69", "tum_1D69__", "phi",
           UnicodeCharacterCategory::GREEK_LETTER_SUBSCRIPT_SMALL},
          {"\u1D6A", "tum_1D6A__", "chi",
           UnicodeCharacterCategory::GREEK_LETTER_SUBSCRIPT_SMALL},
          {"\u2070", "tum_2070__", "0",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUPERSCRIPT},
          {"\u00B9", "tum_00B9__", "1",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUPERSCRIPT},
          {"\u00B2", "tum_00B2__", "2",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUPERSCRIPT},
          {"\u00B3", "tum_00B3__", "3",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUPERSCRIPT},
          {"\u2074", "tum_2074__", "4",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUPERSCRIPT},
          {"\u2075", "tum_2075__", "5",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUPERSCRIPT},
          {"\u2076", "tum_2076__", "6",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUPERSCRIPT},
          {"\u2077", "tum_2077__", "7",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUPERSCRIPT},
          {"\u2078", "tum_2078__", "8",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUPERSCRIPT},
          {"\u2079", "tum_2079__", "9",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUPERSCRIPT},
          {"\u2080", "tum_2080__", "0",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUBSCRIPT},
          {"\u2081", "tum_2081__", "1",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUBSCRIPT},
          {"\u2082", "tum_2082__", "2",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUBSCRIPT},
          {"\u2083", "tum_2083__", "3",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUBSCRIPT},
          {"\u2084", "tum_2084__", "4",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUBSCRIPT},
          {"\u2085", "tum_2085__", "5",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUBSCRIPT},
          {"\u2086", "tum_2086__", "6",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUBSCRIPT},
          {"\u2087", "tum_2087__", "7",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUBSCRIPT},
          {"\u2088", "tum_2088__", "8",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUBSCRIPT},
          {"\u2089", "tum_2089__", "9",
           UnicodeCharacterCategory::ARABIC_NUMBER_SUBSCRIPT}};
      return ucds;
    }  // end of getSupportedUnicodeCharactersDescriptions

    std::string getMangledString(const std::string& s) {
      auto replace_all = [](std::string& v, const char* const s1,
                            const char* const s2) {
        auto pos = std::string::size_type{};
        auto rs = std::string::size_type{};
        auto p = std::string::size_type{};
        auto r = std::string{};
        if ((s1 == nullptr) || (s2 == nullptr)) {
          return;
        }
        const auto ss1 = ::strlen(s1);
        const auto ss2 = ::strlen(s2);
        if (ss1 == 0) {
          return;
        }
        p = v.find(s1, pos);
        if (p == std::string::npos) {
          return;
        }
        while (p != std::string::npos) {
          rs = r.size();
          r.resize(rs + p - pos + ss2);
          std::copy(&v[0] + pos, &v[0] + p, &r[0] + rs);
          std::copy(s2, s2 + ss2, &r[0] + rs + p - pos);
          pos = p + ss1;
          p = v.find(s1, pos);
        }
        rs = r.size();
        r.resize(rs + v.size() - pos);
        std::copy(&v[0] + pos, &v[0] + v.size(), &r[0] + rs);
        v.swap(r);
      };  // end of replace_all
      const auto& ucds = getSupportedUnicodeCharactersDescriptions();
      auto r = s;
      for (const auto& ucd : ucds) {
        replace_all(r, ucd.uc, ucd.m);
      }
      return r;
    }  // end of getMangledString

  }  // end of namespace unicode

}  // end of namespace tfel
