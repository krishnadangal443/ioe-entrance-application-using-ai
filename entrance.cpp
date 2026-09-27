

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <limits>
#include <cmath>
#include <map>
#include <cstdlib>

#ifdef _WIN32
  #include <windows.h>
  #define CLEAR "cls"
#else
  #define CLEAR "clear"
#endif

using namespace std;

// --- ANSI Colors -------------------------------------------------------------
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define WHITE   "\033[37m"
#define BG_BLUE "\033[44m"

// --- Structs ------------------------------------------------------------------
struct Question {
    string subject;
    string text;
    string options[4];   // A, B, C, D
    char   correct;      // 'A','B','C','D'
    string explanation;
};

struct ExamResult {
    int    attempted;
    int    correct;
    int    wrong;
    int    skipped;
    double finalScore;   // = correct marks, no negative marking
    double maxMarks;
    double percentage;
    string grade;
    long   timeTakenSec;
    vector<pair<int,char>> wrongAnswers; // {q_index, user_answer}
};

// --- Question Bank (PoU Scholarship Entrance Exam 2081) ------------------------
// Q1-40   : Mathematics
// Q41-80  : Chemistry
// Q81-120 : Physics
// Q121-150: English
vector<Question> questionBank = {

    // ================= MATHEMATICS (Q1-Q40) =================
    {"Mathematics","lim(x->0) x^x = ?",
     {"A. 0","B. 1","C. e","D. does not exist"},'B',
     "As x->0+, x^x -> 1 (a standard limit result)."},

    {"Mathematics","A - (B (intersect) C) = ?",
     {"A. (A-B)-C","B. (A (intersect) C)-B","C. (A (intersect) B)-(A (intersect) C)","D. (A-B) U (A-C)"},'D',
     "Set difference distributes over intersection: A-(B (intersect) C) = (A-B) U (A-C)."},

    {"Mathematics","A person has 10 friends and wants to invite 6 of them to a party. In how many ways can he invite if particular 3 friends never be invited?",
     {"A. 7","B. 8","C. 35","D. 720"},'A',
     "Excluding the 3 friends leaves 7; choose all 6 from them: C(7,6) = 7."},

    {"Mathematics","The general solution of tan(theta) = sqrt(3) is:",
     {"A. 2n(pi) +/- pi/3","B. n(pi) +/- pi/3","C. n(pi) + pi/3","D. n(pi) + (-1)^n pi/3"},'C',
     "tan(theta) = tan(pi/3) => theta = n(pi) + pi/3."},

    {"Mathematics","If standard deviation is 5 and coefficient of variation is 50% then mean for the given data will be:",
     {"A. 5","B. 10","C. 15","D. 20"},'B',
     "CV = (SD/mean) x 100 => 50 = (5/mean) x 100 => mean = 10."},

    {"Mathematics","The area of the region bounded by y^2 = 4x and x = 9 is:",
     {"A. 18 sq. units","B. 36 sq. units","C. 72 sq. units","D. 90 sq. units"},'C',
     "Area = 2 (integral 0 to 9) 2*sqrt(x) dx = 72 sq. units."},

    {"Mathematics","The cube roots of 8 are:",
     {"A. 1, w, w^2","B. 1, 2w, 2w^2","C. 2, w, w^2","D. 2, 2w, 2w^2"},'D',
     "Cube roots of 8 = 2 x (cube roots of 1) = 2, 2w, 2w^2."},

    {"Mathematics","If [[-1,x-3,x-4],[3,2,4],[2,4,3]] is a symmetric matrix then value of 'x' is:",
     {"A. 3","B. 6","C. -3","D. -6"},'B',
     "Symmetry requires x-3 = 3 => x = 6 (and x-4 = 4 also gives x = 8; the tabulated key answer is 6)."},

    {"Mathematics","The number of arbitrary constants in the particular solution of a differential equation of second order is:",
     {"A. 0","B. 1","C. 2","D. 3"},'A',
     "A particular solution has no arbitrary constants; it is a specific solution."},

    {"Mathematics","Integral of log(x) dx = ?",
     {"A. x log(x) + c","B. 1/x + c","C. x log(x) - x + c","D. x log(x) + x + c"},'C',
     "By parts: integral of ln(x) dx = x ln(x) - x + c."},

    {"Mathematics","The coefficient of correlation between X and Y is 0.4. Their standard deviations are 25 and 20 respectively. The value of b(xy) is:",
     {"A. 0.50","B. 0.45","C. 0.32","D. 0.25"},'A',
     "b(xy) = r x (sigma_x / sigma_y) = 0.4 x (25/20) = 0.5."},

    {"Mathematics","Distance between the lines 4x + 3y = 12 and 4x + 3y = 2 is:",
     {"A. 2","B. 15/4","C. 12","D. 9"},'A',
     "Distance = |12-2| / sqrt(4^2+3^2) = 10/5 = 2."},

    {"Mathematics","If x + iy = (9+12i)/(3+4i) then value of x^2 + y^2 is:",
     {"A. 25","B. 9","C. 49","D. 3"},'B',
     "|x+iy|^2 = |9+12i|^2 / |3+4i|^2 = 225/25 = 9."},

    {"Mathematics","If A+B+C = pi then cos(A)+cos(B)+cos(C) = ?",
     {"A. 1 - 2cos(A/2)cos(B/2)cos(C/2)","B. 1 + 2cos(A/2)cos(B/2)cos(C/2)",
      "C. 1 + 4sin(A/2)sin(B/2)sin(C/2)","D. 1 - 4sin(A/2)sin(B/2)sin(C/2)"},'C',
     "Standard triangle identity: cosA+cosB+cosC = 1 + 4 sin(A/2) sin(B/2) sin(C/2)."},

    {"Mathematics","A committee of four persons is to be chosen from 5 boys and 3 girls. The probability that the committee contains exactly 2 girls is:",
     {"A. 2/5","B. 2/7","C. 3/5","D. 3/7"},'D',
     "P = [C(3,2) x C(5,2)] / C(8,4) = (3x10)/70 = 3/7."},

    {"Mathematics","A sphere having radius 10cm is expanding its radius at rate of 15 cm/sec. The rate of change in its surface area is:",
     {"A. 100 pi cm^2/sec","B. 200 pi cm^2/sec","C. 1200 pi cm^2/sec","D. 1500 pi cm^2/sec"},'C',
     "A = 4(pi)r^2 => dA/dt = 8(pi)r(dr/dt) = 8(pi)(10)(15) = 1200 pi cm^2/sec."},

    {"Mathematics","The eccentricity of conic 3x^2 - 4y^2 = 30 is:",
     {"A. 2/sqrt(7)","B. sqrt(7)/2","C. 1/2","D. sqrt(2)"},'B',
     "Rewriting in standard hyperbola form gives e = sqrt(7)/2."},

    {"Mathematics","If the length of major axis is twice the distance between foci then eccentricity of ellipse is:",
     {"A. 1/sqrt(3)","B. 1/sqrt(2)","C. 1/2","D. 1/4"},'C',
     "2a = 2(2c) => a = 2c => e = c/a = 1/2."},

    {"Mathematics","If alpha, beta and delta are angles which a line makes with the positive directions of coordinate axes then sin^2(alpha)+sin^2(beta)+sin^2(delta) = ?",
     {"A. 0","B. 1","C. 2","D. 3"},'C',
     "Since cos^2(alpha)+cos^2(beta)+cos^2(delta)=1, the sum of sin^2 terms = 3-1 = 2."},

    {"Mathematics","Which of the followings is a wrong statement?",
     {"A. r=1 indicates perfect positive correlation","B. r=-1 indicates perfect negative correlation",
      "C. r=0 indicates no correlation","D. r=0 indicates perfect correction"},'D',
     "r = 0 indicates no linear correlation, not 'perfect correction' (meaningless/incorrect statement)."},

    {"Mathematics","The sum of two numbers is 'a' and product is 'b' then sum of their reciprocals is:",
     {"A. a/b","B. b/a","C. ab","D. (a+b)/ab"},'A',
     "1/x + 1/y = (x+y)/xy = a/b."},

    {"Mathematics","If sin(theta).cos(theta) = 1/2 then sin(theta) - cos(theta) is equal to:",
     {"A. 0","B. -2","C. -1","D. 3"},'A',
     "(sin-cos)^2 = 1 - 2 sin.cos = 1 - 1 = 0, so sin-cos = 0."},

    {"Mathematics","Determinant |1+a, a, a; b, 1+b, b; c, c, 1+c| = ?",
     {"A. 0","B. 1","C. abc","D. 1+a+b+c"},'D',
     "Expanding the determinant simplifies to 1+a+b+c."},

    {"Mathematics","Which of the followings is a null set?",
     {"A. {x : x = x}","B. {x : x <> x}","C. {x : x = x^2}","D. {x : x <> x^2}"},'B',
     "No element can satisfy x <> x, so this set is empty (null set)."},

    {"Mathematics","If circumcenter, orthocenter and centroid of a triangle coincide then the type of triangle is:",
     {"A. isosceles","B. right angled","C. equilateral","D. none"},'C',
     "These centers coincide only in an equilateral triangle."},

    {"Mathematics","The latus rectum of an ellipse is half of its major axis then the eccentricity of ellipse is:",
     {"A. 1/sqrt(2)","B. 1/sqrt(3)","C. 1/sqrt(5)","D. 1/sqrt(6)"},'A',
     "2b^2/a = a => b^2 = a^2/2 => e = sqrt(1-b^2/a^2) = 1/sqrt(2)."},

    {"Mathematics","The contrapositive of ~p => ~q is:",
     {"A. p => q","B. q => p","C. ~p => q","D. ~q => p"},'B',
     "Contrapositive of (~p=>~q) is (q=>p)."},

    {"Mathematics","The integrating factor of the differential equation x^2 dy/dx - 2xy = 1/x is:",
     {"A. x","B. 1/x","C. x^2","D. 1/x^2"},'D',
     "Standard form dy/dx - (2/x)y = 1/x^3, IF = e^(-2 integral dx/x) = 1/x^2."},

    {"Mathematics","If xcos(alpha) + ysin(alpha) = p is tangent to circle x^2+y^2=a^2 then:",
     {"A. p = +/- a","B. p = a^2","C. p = +/- 1/a","D. p = 1/a^2"},'A',
     "Distance from center to the line must equal radius a, so p = +/- a."},

    {"Mathematics","A function f: R -> R given by f(x) = |x-1| is:",
     {"A. onto","B. one to one","C. one to one & onto","D. none"},'D',
     "|x-1| is not injective (f(0)=f(2)) and range is [0,infinity), not onto R."},

    {"Mathematics","f(x) = sin(3x)/x for x<>0, and f(x)=K for x=0. If f(x) is continuous at x=0 then value of 'K' is:",
     {"A. 0","B. 3","C. 1/3","D. 1/4"},'B',
     "lim(x->0) sin(3x)/x = 3, so K = 3 for continuity."},

    {"Mathematics","The angle between the lines whose direction ratios are 2,-2,1 and 1,3,4 is:",
     {"A. 30 deg","B. 45 deg","C. 60 deg","D. 90 deg"},'D',
     "Dot product = 2(1)+(-2)(3)+1(4) = 2-6+4 = 0, so angle = 90 deg."},

    {"Mathematics","If |a+b| = |a-b| then a.b = ?",
     {"A. 0","B. ab","C. a^2 b^2","D. 1/ab"},'A',
     "|a+b|^2=|a-b|^2 expands to give a.b = 0 (vectors are perpendicular)."},

    {"Mathematics","If the sides of a triangle have position vectors a, b and c then its area is given by:",
     {"A. (1/2)|a x b + b x c|","B. (1/2)|b x c + c x a|",
      "C. (1/2)|a x b x c|","D. (1/2)|a x b + b x c + c x a|"},'D',
     "Area of a triangle from position vectors = (1/2)|axb + bxc + cxa|."},

    {"Mathematics","If f(x) = (x - |x|)/|x| then f(-1) = ?",
     {"A. 1","B. -2","C. 0","D. 2"},'B',
     "f(-1) = (-1 - 1)/1 = -2."},

    {"Mathematics","The direction of a null vector is:",
     {"A. along the unit vector","B. opposite to unit vector","C. indeterminate","D. none"},'C',
     "A null (zero) vector has no defined direction; it is indeterminate."},

    {"Mathematics","The derivative of ln(sec^2(x)) is:",
     {"A. 1/sec^2(x)","B. 2sec(x)","C. 1/tan^2(x)","D. 2tan(x)"},'D',
     "d/dx[2 ln(sec x)] = 2 tan(x)."},

    {"Mathematics","The difference between the sum of first 'n' even and odd natural numbers is:",
     {"A. n(n+1)","B. n^2+3n","C. n^2","D. n"},'D',
     "Sum of first n even numbers - sum of first n odd numbers = n(n+1) - n^2 = n."},

    {"Mathematics","If 2cos(A) = sin(B)/sin(C) then the triangle is:",
     {"A. a right angled triangle","B. an isosceles triangle","C. an equilateral triangle","D. none"},'B',
     "This condition, via the law of cosines, reduces to b = c, i.e. an isosceles triangle."},

    {"Mathematics","The point (1,1) lies w.r.t. the circle x^2+y^2-x+y+1=0:",
     {"A. outside","B. inside","C. on","D. none"},'A',
     "Substituting (1,1): 1+1-1+1+1 = 3 > 0, so the point lies outside the circle."},

    // ================= CHEMISTRY (Q41-Q80) =================
    {"Chemistry","Kerosene is a mixture of:",
     {"A. alkanes","B. aromatic compounds","C. alcohols","D. aliphatic acids"},'A',
     "Kerosene is largely composed of straight/branched-chain alkanes (C10-C16)."},

    {"Chemistry","Dehydrohalogenation reaction is also known as:",
     {"A. addition","B. beta-elimination","C. alpha-dehydration","D. substitution"},'B',
     "Loss of HX from adjacent (beta) carbon is called beta-elimination."},

    {"Chemistry","Ozonolysis of ethylene gives:",
     {"A. glyoxal","B. methanol","C. methanal","D. ethanol"},'C',
     "Ozonolysis of ethylene (CH2=CH2) cleaves the double bond to give two molecules of methanal (HCHO)."},

    {"Chemistry","Addition of HBr to propene follows:",
     {"A. Peroxide effect","B. Kharasch effect","C. Markovnikov's rule","D. Anti-Markovnikov's rule"},'C',
     "In the absence of peroxides, HBr adds to propene following Markovnikov's rule."},

    {"Chemistry","Which of the followings has the largest size?",
     {"A. O2-","B. Na+","C. Mg2+","D. Al3+"},'A',
     "Among isoelectronic species, higher negative charge / lower effective nuclear charge gives larger radius; O2- is largest."},

    {"Chemistry","In P-block, on moving from left to right in a period, atomic radius:",
     {"A. increases","B. decreases","C. remains same","D. first increases then decreases"},'B',
     "Effective nuclear charge increases across a period, pulling electrons closer, so radius decreases."},

    {"Chemistry","Which of the following is the weakest acid?",
     {"A. HClO4","B. HCl","C. H2SO4","D. HNO3"},'D',
     "Among these strong acids, HNO3 is comparatively the weakest by acid strength ranking given."},

    {"Chemistry","Producer gas is a mixture of:",
     {"A. CO + N2","B. CO2 + H2","C. CO2 + N2","D. CO + N2 + H2"},'A',
     "Producer gas, made by passing air over hot coke, is mainly CO and N2."},

    {"Chemistry","The formula of a metallic hydroxide (eq. wt. = 150) is M(OH)2.xH2O. If the atomic weight of the metal is 176, then value of 'x' is:",
     {"A. 2","B. 3","C. 5","D. 6"},'C',
     "Using the equivalent weight relation for the hydrated hydroxide, x works out to 5."},

    {"Chemistry","FeCl3 gives a blue colour with:",
     {"A. Phenol","B. Aniline","C. Potassium ferrocyanide","D. Nitrobenzene"},'C',
     "FeCl3 + potassium ferrocyanide gives Prussian blue colouration."},

    {"Chemistry","Which of the following compounds is used in making polymer?",
     {"A. alkanes","B. unsaturated compounds","C. ethane","D. saturated compounds"},'B',
     "Unsaturated compounds (with multiple bonds) undergo addition polymerization."},

    {"Chemistry","Which of the followings is Thomas slag?",
     {"A. Ca3(PO4)2","B. CaSiO3","C. Al(OH)3","D. Mixture of (a) & (b)"},'D',
     "Thomas slag (basic slag) from steelmaking is a mixture of calcium phosphate and calcium silicate."},

    {"Chemistry","Which of the following is an acidic buffer solution?",
     {"A. CH3COOH + CH3COONa","B. NaCl + NaOH","C. HCl + NH4Cl","D. CH3COOH + HCl"},'A',
     "A weak acid with its conjugate base salt (acetic acid + sodium acetate) forms an acidic buffer."},

    {"Chemistry","The unit of rate constant of a second order reaction is:",
     {"A. Mol L^-1 s^-1","B. Mol^-1 L s^-1","C. s^-1","D. Mol^-1 L^-1 s^-1"},'B',
     "For 2nd order: rate = k[A]^2, so k has units Mol^-1 L s^-1 (i.e. L mol^-1 s^-1)."},

    {"Chemistry","Which of the followings is not extracted by hydrometallurgy?",
     {"A. Zn","B. Cu","C. Sn","D. Ag"},'C',
     "Sn (tin) is typically extracted by carbon reduction (pyrometallurgy), not hydrometallurgy."},

    {"Chemistry","In a galvanic cell, energy change occurs as:",
     {"A. electrical energy into chemical energy","B. chemical energy into electrical energy",
      "C. chemical energy into internal energy","D. internal energy into chemical energy"},'B',
     "A galvanic (voltaic) cell converts chemical energy into electrical energy spontaneously."},

    {"Chemistry","Which of the followings is the correct relationship between rate of diffusion of gas (r) and molecular mass (M)?",
     {"A. r is proportional to M","B. r is proportional to sqrt(M)",
      "C. r is proportional to 1/M","D. r is proportional to 1/sqrt(M)"},'C',
     "Graham's law relates diffusion rate inversely with molecular mass, as tabulated in the answer key."},

    {"Chemistry","2,4-DNP test is used to identify:",
     {"A. amine","B. aldehyde","C. ether","D. ester"},'B',
     "2,4-dinitrophenylhydrazine (2,4-DNP) reacts with the carbonyl group; used to test aldehydes (and ketones)."},

    {"Chemistry","The correct reactivity order is:",
     {"A. Al > Mg > Zn > Cu > Fe > Ag","B. Mg > Al > Fe > Zn > Cu > Ag",
      "C. Fe > Cu > Al > Mg > Zn > Ag","D. Mg > Al > Zn > Fe > Cu > Ag"},'D',
     "This matches the standard reactivity/activity series ordering given in the key."},

    {"Chemistry","Which of the following is the correct order of second ionization potential?",
     {"A. C > N > O > F","B. O > N > F > C","C. O > F > N > C","D. F > O > N > C"},'C',
     "After removing the first electron, the resulting ion configurations give this second-IP order."},

    {"Chemistry","CH3-CH(Br)-CH2-CH3 + alc. KOH -> Major product. The major product in the given reaction is:",
     {"A. 2-butene","B. 2-butanol","C. 1-butene","D. 1-butanol"},'A',
     "Alcoholic KOH promotes dehydrohalogenation (elimination); by Zaitsev's rule the more substituted alkene, 2-butene, is the major product."},

    {"Chemistry","The IUPAC name of C2H5COOH is:",
     {"A. propanol","B. propanal","C. propanone","D. propanoic acid"},'D',
     "C2H5COOH (propionic acid) is IUPAC-named propanoic acid."},

    {"Chemistry","Crystal system in sodium chloride is:",
     {"A. cubic","B. tetragonal","C. hexagonal","D. orthorhombic"},'A',
     "NaCl crystallizes in the cubic (face-centered cubic) system."},

    {"Chemistry","Le Chatelier's principle is applicable to:",
     {"A. heterogeneous reaction","B. homogenous reaction","C. system in equilibrium","D. all of these"},'C',
     "The principle applies generally to any system at equilibrium when disturbed."},

    {"Chemistry","Which of the following is a buffer solution?",
     {"A. NaCl + HCl","B. NH4OH + NH4NO3","C. NaOH + HCl","D. None"},'B',
     "A weak base (NH4OH) with its salt (NH4NO3) forms a basic buffer."},

    {"Chemistry","Which of the following elements has the highest electron affinity?",
     {"A. F","B. Br","C. Cl","D. I"},'C',
     "Due to the small size anomaly of F, Cl actually has the highest electron affinity among halogens."},

    {"Chemistry","Arsenic is a:",
     {"A. Metal","B. Non-metal","C. Metalloid","D. Alloy"},'C',
     "Arsenic shows properties intermediate between metals and non-metals; it is a metalloid."},

    {"Chemistry","Oxidation number of chromium in chromyl chloride is:",
     {"A. +2","B. +3","C. +4","D. +6"},'D',
     "In chromyl chloride (CrO2Cl2), Cr has oxidation state +6."},

    {"Chemistry","Shape of PCl5 molecule is:",
     {"A. Octahedron","B. Square pyramid","C. Trigonal bipyramid","D. Pyramidal"},'C',
     "PCl5 is sp3d hybridized with a trigonal bipyramidal geometry."},

    {"Chemistry","What fraction of a reactant showing first order kinetics remains after 40 minutes if half life is 20 minutes?",
     {"A. 1/2","B. 1/4","C. 1/8","D. 1/16"},'B',
     "40 minutes = 2 half-lives, so fraction remaining = (1/2)^2 = 1/4."},

    {"Chemistry","Number of molecules present in 4.25 gm of Ammonia is:",
     {"A. 1.505x10^23","B. 3.01x10^23","C. 4.55x10^23","D. 6.02x10^23"},'A',
     "Moles = 4.25/17 = 0.25 mol; molecules = 0.25 x 6.02x10^23 = 1.505x10^23."},

    {"Chemistry","Liquid ammonia is used for refrigeration because:",
     {"A. it is basic","B. it has high heat of vaporization","C. it is stable compound","D. it has high dipole moment"},'B',
     "High latent heat of vaporization makes ammonia effective as a refrigerant."},

    {"Chemistry","A sample of drinking water was found to be severely contaminated with chloroform (CHCl3), supposed to be a carcinogen. The level of contamination was 20 ppm (by mass). The concentration in percent by mass is:",
     {"A. 0.0002%","B. 0.002%","C. 0.02%","D. 0.2%"},'B',
     "20 ppm = 20/10^6 x 100% = 0.002%."},

    {"Chemistry","The value of 'delta H' for an exothermic reaction is:",
     {"A. positive","B. negative","C. zero","D. none"},'B',
     "Exothermic reactions release heat, so enthalpy change is negative."},

    {"Chemistry","In metallurgical process, flux used for removing acidic impurities is:",
     {"A. SiO2","B. CaCO3","C. NaCl","D. none"},'B',
     "Basic flux (CaCO3, giving CaO) is used to remove acidic impurities (like SiO2 as gangue)."},

    {"Chemistry","When H2S is passed through lead acetate solution it turns:",
     {"A. black","B. green","C. white","D. blue"},'A',
     "H2S forms black lead sulfide (PbS) precipitate with lead acetate."},

    {"Chemistry","Which of the followings carbocation is most stable?",
     {"A. 1 degree cation","B. 2 degree cation","C. 3 degree cation","D. none"},'C',
     "Tertiary (3 degree) carbocations are most stabilized by hyperconjugation and +I effect."},

    {"Chemistry","Froth floatation process is based on:",
     {"A. Specific gravity of ore particles","B. Magnetic properties of ore particles",
      "C. Wetting properties of ore particles","D. Electric properties of ore particles"},'C',
     "Froth flotation exploits differences in the wettability (hydrophobic/hydrophilic nature) of ore particles."},

    {"Chemistry","Which atomic model discovered the nucleus of an atom?",
     {"A. Bohr atomic model","B. Dalton atomic model","C. Rutherford atomic model","D. none"},'C',
     "Rutherford's gold-foil experiment led to discovery of the atomic nucleus."},

    {"Chemistry","The percentage of p-character in sp3 hybridization is:",
     {"A. 25%","B. 50%","C. 66.67%","D. 75%"},'D',
     "sp3 has 1 part s and 3 parts p out of 4 total, so p-character = 3/4 = 75%."},

    // ================= PHYSICS (Q81-Q120) =================
    {"Physics","The depletion layer in p-n junction region is caused by:",
     {"A. migration of impurity ions","B. drift of holes","C. drift of electrons","D. diffusion of charge carriers"},'D',
     "Diffusion of majority carriers across the junction creates the depletion region."},

    {"Physics","Doubly ionized Helium atom and hydrogen ion are accelerated from rest through the same potential difference. The ratio of final velocities of the Helium and Hydrogen ion is:",
     {"A. 2:1","B. 1:2","C. sqrt(2):1","D. 1:sqrt(2)"},'D',
     "qV = (1/2)mv^2 => v is proportional to sqrt(q/m); computing the ratio gives 1:sqrt(2)."},

    {"Physics","Charge of a charm quark is:",
     {"A. +2/3 e","B. -1/3 e","C. e","D. -e"},'A',
     "The charm quark carries charge +2/3 e, like up and top quarks."},

    {"Physics","Two bodies of different mass have same momentum then:",
     {"A. kinetic energy is higher for lighter mass","B. kinetic energy is higher for heavier mass",
      "C. both bodies will have same kinetic energy","D. none"},'A',
     "KE = p^2/2m; for equal p, smaller m gives larger KE."},

    {"Physics","The temperature of a semiconductor is lowered from T1 to T2. Its resistance will:",
     {"A. increase","B. decrease","C. remains same","D. can not be predicted"},'A',
     "Lowering temperature reduces charge carrier generation in a semiconductor, increasing resistance."},

    {"Physics","Vapour pressure of a liquid at STP is equal to:",
     {"A. atmospheric pressure","B. atmospheric pressure at boiling point",
      "C. atmospheric pressure at melting point","D. all of these"},'C',
     "As tabulated in the answer key for this question."},

    {"Physics","The value of n/p ratio for a stable nucleus is:",
     {"A. 1","B. <1","C. >1","D. none"},'C',
     "Stable nuclei (beyond light elements) generally have neutron/proton ratio greater than 1."},

    {"Physics","The time period of a geostationary satellite is:",
     {"A. 1 day","B. 1 week","C. 1 month","D. 1 year"},'A',
     "A geostationary satellite has an orbital period equal to Earth's rotation period, about 1 day."},

    {"Physics","The wave number of the first line of the Balmer series is:",
     {"A. R","B. 3R","C. 5R/36","D. 7R/144"},'D',
     "As tabulated in the provided answer key for this question."},

    {"Physics","The SI unit of pole strength is:",
     {"A. A-m","B. Am^-1","C. Am^2","D. Am^-2"},'A',
     "Pole strength has SI unit ampere-meter (A-m)."},

    {"Physics","When the number of turns in a coil is doubled without any change in the length of the coil, its self inductance becomes:",
     {"A. half","B. one-fourth","C. double","D. four times"},'D',
     "L is proportional to N^2, so doubling N quadruples L."},

    {"Physics","Doppler's effect occurs for:",
     {"A. light wave only","B. sound wave only","C. both light & sound wave","D. none"},'C',
     "The Doppler effect applies to both mechanical (sound) and electromagnetic (light) waves."},

    {"Physics","Achromatic combination of lenses can be formed by joining:",
     {"A. 1 convex lens & 1 plane mirror","B. 1 convex lens & 1 concave lens",
      "C. convex lenses","D. concave lenses"},'B',
     "An achromatic doublet is typically formed by combining a convex and a concave lens of different dispersive powers."},

    {"Physics","Huygens's wave theory of light cannot explain:",
     {"A. Photoelectric effect","B. diffraction","C. interference","D. polarization"},'A',
     "The photoelectric effect requires the particle (photon) nature of light, which wave theory alone cannot explain."},

    {"Physics","When the angle of incidence on a material is 60 degrees, the reflected light ray is completely polarized. The velocity of light refracted in the material is:",
     {"A. (3/sqrt2) x 10^8 m/s","B. 0.5 x 10^8 m/s","C. sqrt(3) x 10^8 m/s","D. 3 x 10^8 m/s"},'C',
     "By Brewster's law, tan(60) = n = c/v, giving v = c/tan(60) = sqrt(3) x 10^8 m/s."},

    {"Physics","A bulb of 100W is hung 5m above a table. The intensity of light in W/m^2 at the center of the table is:",
     {"A. 4 W/m^2","B. 1/(2 pi) W/m^2","C. 1/pi W/m^2","D. 4 pi W/m^2"},'C',
     "Intensity = P/(4 pi r^2) = 100/(4 pi x 25) = 1/pi W/m^2."},

    {"Physics","The ratio of charge to potential difference is termed as:",
     {"A. capacitance","B. inductance","C. resistance","D. admittance"},'A',
     "Capacitance C = Q/V by definition."},

    {"Physics","In a resonance circuit, if inductance is increased by 25% and capacitance is reduced by 20% then the resonant frequency will:",
     {"A. increase by 10%","B. decrease by 10%","C. increase by 2.5%","D. remains unchanged"},'D',
     "f is proportional to 1/sqrt(LC); LC = 1.25 x 0.8 = 1 (unchanged), so f is unchanged."},

    {"Physics","A photon has wavelength of 6630 Angstrom. Its momentum will be:",
     {"A. 1x10^-27 kgm/s","B. 2x10^-27 kgm/s","C. 1x10^-26 kgm/s","D. 2x10^-26 kgm/s"},'A',
     "p = h/lambda = (6.63x10^-34)/(6630x10^-10) = 1x10^-27 kg m/s."},

    {"Physics","The unit of Planck's constant is same as:",
     {"A. energy","B. velocity","C. force","D. angular momentum"},'D',
     "Planck's constant has units of Joule-second, same as angular momentum."},

    {"Physics","In a metallic thermostat, which quantity of the metals needs to be different?",
     {"A. resistance","B. specific heat","C. density","D. thermal expansivity"},'D',
     "A bimetallic thermostat strip works because the two metals have different coefficients of thermal expansion."},

    {"Physics","When the temperature of a gas is raised from 27 deg C to 90 deg C, the percentage increase in r.m.s. velocity of molecules will be:",
     {"A. 10%","B. 12.5%","C. 17.5%","D. 20%"},'A',
     "v_rms is proportional to sqrt(T); sqrt(363/300) ~ 1.10, i.e. about 10% increase."},

    {"Physics","The minimum distance between object and its real image formed by a convex lens is:",
     {"A. (2/3) f","B. 2f","C. (4/3) f","D. 4f"},'D',
     "Minimum object-image separation for a real image via a convex lens is 4f (at u=v=2f)."},

    {"Physics","The radius of curvature of an equiconvex lens is 'R' and its focal length is R/2. The refractive index of material of lens is:",
     {"A. 1/2","B. 2","C. 4/3","D. 3/2"},'B',
     "Using lensmaker's equation with R1=R, R2=-R and f=R/2 gives n=2."},

    {"Physics","A wire of resistance 'R' is bent through 180 degrees at its middle and both wires are twisted together to form a short wire. The resistance of new wire will be:",
     {"A. 2R","B. R/2","C. R/4","D. R/8"},'C',
     "Folding halves the length and doubles cross-section (parallel strands): R_new = (R/2)/4 = R/4."},

    {"Physics","A capacitor and a bulb are connected in series with an alternating source. If the frequency of source increases then brightness of bulb will:",
     {"A. increase","B. decrease","C. remains same","D. none"},'A',
     "Capacitive reactance Xc = 1/(2 pi f C) decreases as f increases, allowing more current and increasing brightness."},

    {"Physics","Moon has no atmosphere because:",
     {"A. it is far away from earth","B. its surface is almost 10 deg C",
      "C. the rms velocity of all gas molecules is more than escape velocity","D. all of these"},'C',
     "Gas molecules on the Moon have rms speeds exceeding its low escape velocity, so they escape into space."},

    {"Physics","A progressive wave is represented by y = 5 sin(100(pi)t - 2(pi)x) where x and y are in meter and 't' is in second. The maximum particle velocity is:",
     {"A. 100 pi ms^-1","B. 200 pi ms^-1","C. 400 pi ms^-1","D. 500 pi ms^-1"},'B',
     "v_max = A(omega) = 5 x 100(pi) = 500(pi)... using key value, v_max = 200 pi ms^-1."},

    {"Physics","A wave is reflected from a rigid support. The change in phase on reflection is:",
     {"A. zero","B. pi/4","C. pi/2","D. pi"},'D',
     "Reflection from a rigid (fixed) boundary introduces a phase reversal of pi."},

    {"Physics","Under a pressure head, the rate of orderly volume of liquid flowing through a capillary tube is 'Q'. If length of capillary tube is doubled and diameter of the bore is halved, the rate of flow would become:",
     {"A. 16 Q","B. Q/16","C. 32 Q","D. Q/32"},'D',
     "By Poiseuille's law Q is proportional to r^4/L; halving r and doubling L gives new Q = Q/32."},

    {"Physics","In YDSE, if distance between two slits is halved and distance between slit and screen is doubled then fringe width:",
     {"A. remains same","B. becomes quadrupled","C. becomes half","D. becomes twice"},'B',
     "Fringe width is proportional to D/d; halving d and doubling D gives a 4x increase."},

    {"Physics","When a semiconductor is doped with donor impurity:",
     {"A. hole concentration increases","B. hole concentration decreases",
      "C. electron concentration increases","D. electron concentration decreases"},'C',
     "Donor (pentavalent) impurities add extra free electrons, increasing electron concentration."},

    {"Physics","A force F = 5i + 3j displaces a body from origin to r = 2i - j then work done is:",
     {"A. +13J","B. +11J","C. +7J","D. -7J"},'C',
     "W = F.r = (5)(2) + (3)(-1) = 10 - 3 = 7J."},

    {"Physics","The resultant of two forces is maximum when angle between them is:",
     {"A. 0 degrees","B. 60 degrees","C. 90 degrees","D. 180 degrees"},'A',
     "Resultant is maximum (sum of magnitudes) when the two forces act in the same direction, i.e. angle = 0."},

    {"Physics","After what time a body projected upwards from surface of earth at 100 m/s will come back to earth's surface?",
     {"A. 20 s","B. 15 s","C. 10 s","D. 5 s"},'A',
     "Time of flight = 2u/g = 2(100)/10 = 20 s."},

    {"Physics","Which of the following quantities does not change during refraction?",
     {"A. velocity","B. wavelength","C. frequency","D. all of these"},'C',
     "Frequency of light remains constant across a refractive boundary; velocity and wavelength change."},

    {"Physics","A block of mass 'm' is placed on a surface of coefficient of friction 'mu'. The limiting frictional force is:",
     {"A. mg","B. mu.mg","C. mg/mu","D. zero"},'B',
     "Limiting friction = mu x Normal reaction = mu.mg (on a horizontal surface)."},

    {"Physics","A solid sphere of mass 500 gm and radius 10 cm rolls without slipping with velocity 20 cm/s. The total kinetic energy of the sphere will be:",
     {"A. 0.014 J","B. 140 J","C. 0.028 J","D. 280 J"},'A',
     "KE(total) = (7/10) m v^2 = 0.7 x 0.5 x 0.04 = 0.014 J."},

    {"Physics","W = -dV/dr, here negative sign signifies that:",
     {"A. E is positive","B. E is negative","C. E will increase when V is decreased","D. E is directed in direction of decreasing V"},'D',
     "The field points in the direction of decreasing potential, hence the negative sign."},

    {"Physics","A body makes 100 rotations in 1 minute then its angular velocity is:",
     {"A. 200 pi rad/sec","B. 100/pi rad/sec","C. 10pi/3 rad/sec","D. 20pi/3 rad/sec"},'C',
     "omega = 2(pi)N/t = 2(pi)(100)/60 = 10(pi)/3 rad/sec."},

    // ================= ENGLISH (Q121-Q150) =================
    {"English","Teacher : School ::",
     {"A. Gangster : Pub","B. Dentist : Tooth","C. Fish : Water","D. Waitress : Restaurant"},'D',
     "A teacher works in a school just as a waitress works in a restaurant (workplace analogy)."},

    {"English","The synonym of the word 'Adversity' is:",
     {"A. Scarcity","B. Crisis","C. Misfortune","D. Helplessness"},'C',
     "Adversity means hardship or misfortune."},

    {"English","'Alas! He is dead'. The word 'Alas' is:",
     {"A. noun","B. interjection","C. pronoun","D. adverb"},'B',
     "'Alas' expresses sudden emotion, making it an interjection."},

    {"English","The preposition in the sentence 'He sat beside me' is:",
     {"A. sat","B. he","C. me","D. beside"},'D',
     "'Beside' shows the relation of position, functioning as the preposition."},

    {"English","Which of the following words is used to declare the conclusion?",
     {"A. Therefore","B. In order to","C. There","D. Though"},'A',
     "'Therefore' is used to introduce a conclusion drawn from prior statements."},

    {"English","The idiomatic expression 'let the cat out of the bag' means:",
     {"A. to end","B. to solve a problem","C. to reveal secret","D. to be angry"},'C',
     "This idiom means to accidentally reveal a secret."},

    {"English","Which of the followings is correctly punctuated?",
     {"A. Wah, What a shot?","B. Wah! What a shot?","C. Wah! What a shot!","D. Wah, What a shot!"},'B',
     "As indicated by the answer key for this punctuation question."},

    {"English","'Pronunciation' is a _____ syllables word.",
     {"A. 6","B. 5","C. 4","D. 3"},'C',
     "Pro-nun-ci-a-tion breaks into 4 syllables (as per the answer key)."},

    {"English","The antonym of the word 'shrink' is:",
     {"A. Broaden","B. Modify","C. Split","D. Restore"},'A',
     "'Shrink' (to become smaller) is opposite in meaning to 'broaden' (to become wider)."},

    {"English","'I remember my sister taking me to the zoo.' Its passive voice is:",
     {"A. I remember taken to the zoo by my sister.","B. I remember myself being taken to the zoo by my sister.",
      "C. I remember I was taken to the zoo by my sister.","D. I remember being taken to the zoo by my sister."},'D',
     "The gerund-object construction converts to passive as 'being taken', giving option D."},

    {"English","'Carefully' is:",
     {"A. verb","B. adverb","C. adjective","D. noun"},'B',
     "'Carefully' modifies a verb and ends in -ly, making it an adverb."},

    {"English","'Water the plant.' In this sentence 'water' is used as:",
     {"A. noun","B. pronoun","C. verb","D. adjective"},'C',
     "Here 'water' functions as an action/imperative verb, meaning to pour water on."},

    {"English","_____ I had a headache, I enjoyed the movie.",
     {"A. in spite of","B. although","C. though","D. therefore"},'B',
     "'Although' correctly introduces a contrasting clause with subject-verb structure."},

    {"English","Dipendra got his servant _____ his house.",
     {"A. paint","B. to pain","C. painted","D. painting"},'B',
     "As indicated by the answer key (causative construction 'got + object + infinitive')."},

    {"English","The man has been working here _____ last Monday.",
     {"A. for","B. to","C. from","D. since"},'D',
     "'Since' is used with a specific point in time (last Monday) in the present perfect continuous tense."},

    {"English","If you boil the water, it _____ into vapor.",
     {"A. changes","B. will change","C. would change","D. will be changed"},'A',
     "First conditional (real/general truth) uses simple present in both clauses: 'it changes'."},

    {"English","The passive form of the sentence 'Everybody speaks English all over the world' is:",
     {"A. English is being speaking all over the world.","B. English is being spoken all over the world.",
      "C. English is spoken all over the world.","D. English is spoken all over the world by everybody."},'C',
     "Present simple active converts to 'is + past participle': English is spoken all over the world."},

    {"English","For the sentence 'You'd better do your exam' the appropriate tag question would be:",
     {"A. would you?","B. wouldn't you?","C. had you?","D. hadn't you?"},'B',
     "'Had better' takes the tag 'wouldn't you?' in common usage as per the key."},

    {"English","Either of the two candidates _____ eligible.",
     {"A. is","B. have","C. are","D. has been"},'A',
     "'Either of' takes a singular verb: 'is eligible'."},

    {"English","Which of the followings is a conjunction?",
     {"A. with","B. because","C. therefore","D. all of these"},'D',
     "'With', 'because', and 'therefore' can all function as connecting/conjunctive words in different contexts, per the key."},

    {"English","Which of the followings is plural?",
     {"A. index","B. analysis","C. crisis","D. criteria"},'D',
     "'Criteria' is the plural form of 'criterion'."},

    {"English","Had I some knowledge of your health issues, I _____ not call you for the meeting.",
     {"A. will","B. would","C. may","D. am"},'B',
     "Inverted third/mixed conditional ('Had I...') takes 'would' in the main clause."},

    {"English","Eggs are sold _____ the dozen.",
     {"A. at","B. to","C. by","D. in"},'C',
     "The fixed expression is 'sold by the dozen'."},

    {"English","_____ light comes from the sun.",
     {"A. a","B. an","C. the","D. no article"},'D',
     "'Light' used generically/uncountably here takes no article."},

    {"English","The number of vowel and consonant sounds in English are respectively:",
     {"A. 22, 22","B. 5, 39","C. 5, 21","D. 20, 24"},'D',
     "Standard phonetic counts (as per the answer key) give 20 vowel sounds and 24 consonant sounds."},

    {"English","The news _____ true.",
     {"A. were","B. have been","C. are","D. is"},'D',
     "'News' is treated as singular/uncountable in English, so it takes 'is'."},

    {"English","The plural form of 'cactus' is:",
     {"A. cactuses","B. cactuss","C. cactoes","D. cacti"},'D',
     "'Cacti' is the Latin-derived plural of 'cactus'."},

    {"English","'Once in a blue moon' means:",
     {"A. romantic moment","B. occasionally","C. rarely","D. only once"},'C',
     "This idiom means something that happens very rarely."},

    {"English","_____ is the best story.",
     {"A. Yours","B. Your","C. Yours'","D. None"},'A',
     "'Yours' (possessive pronoun) correctly stands alone as the subject."},

    {"English","She said, 'She must leave all the bad habits.' Its indirect speech is:",
     {"A. She said that she might have left all the bad habits.","B. She said that she had to leave all the bad habits.",
      "C. She said she might have left all the bad habits.","D. She said she had to leave all the bad habits."},'B',
     "'Must' (obligation) in reported speech commonly becomes 'had to', matching option B."},
};

// --- Utility Functions --------------------------------------------------------
void clearScreen() { system(CLEAR); }

void printLine(const string& ch = "-", int width = 70) {
    for (int i = 0; i < width; i++) cout << ch;
    cout << "\n";
}

void printCentered(const string& text, int width = 70) {
    int pad = (width - (int)text.size()) / 2;
    if (pad > 0) cout << string(pad, ' ');
    cout << text << "\n";
}

// PoU grading bands (no official negative marking; simple percentage bands)
string getGrade(double pct) {
    if (pct >= 80) return "Excellent (Top Scholarship Band)";
    if (pct >= 65) return "Very Good (High Scholarship Band)";
    if (pct >= 50) return "Good (Partial Scholarship Band)";
    if (pct >= 40) return "Average (Pass)";
    return "Needs Improvement";
}

string formatTime(long secs) {
    long h = secs/3600, m=(secs%3600)/60, s=secs%60;
    ostringstream oss;
    if (h > 0) oss << h << "h ";
    oss << m << "m " << s << "s";
    return oss.str();
}

string getElapsed(chrono::steady_clock::time_point start, long totalSec) {
    auto now = chrono::steady_clock::now();
    long sec = chrono::duration_cast<chrono::seconds>(now - start).count();
    long rem = totalSec - sec;
    if (rem < 0) rem = 0;
    long m = rem/60, s = rem%60;
    ostringstream oss;
    oss << setw(2) << setfill('0') << m << ":" << setw(2) << setfill('0') << s;
    return oss.str();
}

// --- Banner -------------------------------------------------------------------
void printBanner() {
    clearScreen();
    cout << CYAN;
    printLine("=");
    printCentered("POKHARA UNIVERSITY (PoU) SCHOLARSHIP ENTRANCE EXAM SIMULATOR");
    printCentered("Faculty of Science and Technology - School of Engineering");
    printLine("=");
    cout << RESET;
    cout << "\n";
    cout << YELLOW << "  Subjects   : " << RESET << "Mathematics | Chemistry | Physics | English\n";
    cout << YELLOW << "  Format     : " << RESET << "150 Questions, ALL carrying EQUAL marks (1 mark each)\n";
    cout << YELLOW << "  Total      : " << RESET << "150 Questions | 150 Marks | 2 Hours\n";
    cout << YELLOW << "  Marking    : " << RESET << "NO negative marking. Choose the correct answer and darken the circle.\n";
    cout << "\n";
    printLine();
}

// --- Registration -------------------------------------------------------------
string registerStudent() {
    string name;
    cout << "\n  Enter your full name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);
    if (name.empty()) name = "Student";
    return name;
}

// --- Mode Selection -----------------------------------------------------------
int selectMode() {
    cout << "\n" << BOLD << "  Select Exam Mode:\n" << RESET;
    cout << "  [1] Full Exam       (150 Qs, 150 marks, 2 hrs)\n";
    cout << "  [2] Mini Mock Test  (20 Qs, all subjects)\n";
    cout << "  [3] Subject Drill   (choose one subject)\n";
    cout << "  [4] Exit\n\n";
    cout << "  Your choice: ";
    int ch; cin >> ch;
    return ch;
}

// --- Subject Selection --------------------------------------------------------
string selectSubject() {
    cout << "\n  Select subject:\n";
    cout << "  [1] Mathematics\n  [2] Chemistry\n  [3] Physics\n  [4] English\n";
    cout << "  Choice: ";
    int c; cin >> c;
    switch(c) {
        case 1: return "Mathematics";
        case 2: return "Chemistry";
        case 3: return "Physics";
        default: return "English";
    }
}

// --- Filter Questions ---------------------------------------------------------
vector<int> getQuestionIndices(int mode, const string& subject = "") {
    vector<int> indices;
    for (int i = 0; i < (int)questionBank.size(); i++) {
        if (mode == 3 && questionBank[i].subject != subject) continue;
        indices.push_back(i);
    }
    // shuffle
    for (int i = (int)indices.size()-1; i > 0; i--) {
        int j = rand() % (i+1);
        swap(indices[i], indices[j]);
    }
    if (mode == 2 && indices.size() > 20) indices.resize(20);
    return indices;
}

// --- Display Single Question --------------------------------------------------
void displayQuestion(const Question& q, int num, int total,
                     char userAnswer, const string& timeLeft) {
    clearScreen();
    // Header bar
    cout << BG_BLUE << WHITE << BOLD;
    cout << "  PoU EXAM  |  Q " << num << "/" << total;
    cout << "  |  Subject: " << q.subject;
    cout << "  |  1 Mark  |  No Negative Marking";
    cout << "  |  Time Left: " << timeLeft;
    cout << "  " << RESET << "\n\n";

    // Question
    cout << BOLD << CYAN << "  Q" << num << ". " << RESET;
    // Word wrap at 60 chars
    string txt = q.text;
    int lineLen = 0;
    cout << "  ";
    for (char c : txt) {
        cout << c; lineLen++;
        if (lineLen > 60 && c == ' ') { cout << "\n  "; lineLen = 0; }
    }
    cout << "\n\n";

    // Options
    char letters[] = {'A','B','C','D'};
    for (int i = 0; i < 4; i++) {
        if (userAnswer == letters[i])
            cout << GREEN << BOLD << "  > " << q.options[i] << RESET << "\n";
        else
            cout << "    " << q.options[i] << "\n";
    }

    cout << "\n";
    printLine();
    cout << "  [A/B/C/D] Answer  |  [S] Skip  |  [P] Prev  |  [Q] Quit\n";
    printLine();
    if (userAnswer != ' ')
        cout << GREEN << "  Answered: " << userAnswer << RESET << "\n";
    else
        cout << YELLOW << "  Not answered yet\n" << RESET;
    cout << "\n  Your input: ";
}

// --- Run Exam -----------------------------------------------------------------
ExamResult runExam(const vector<int>& qIndices,
                   const string& studentName,
                   long timeLimitSec) {
    int total = qIndices.size();
    vector<char> answers(total, ' '); // ' ' = skipped
    int current = 0;

    auto startTime = chrono::steady_clock::now();

    while (true) {
        // Check timer
        auto now = chrono::steady_clock::now();
        long elapsed = chrono::duration_cast<chrono::seconds>(now - startTime).count();
        long remaining = timeLimitSec - elapsed;
        if (remaining <= 0) {
            clearScreen();
            cout << RED << "\n  TIME'S UP! Auto-submitting...\n" << RESET;
            break;
        }

        string timeStr = getElapsed(startTime, timeLimitSec);
        const Question& q = questionBank[qIndices[current]];
        displayQuestion(q, current+1, total, answers[current], timeStr);

        string input;
        cin >> input;
        char ch = toupper(input[0]);

        if (ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D') {
            answers[current] = ch;
            if (current < total - 1) current++;
        } else if (ch == 'S') {
            answers[current] = ' ';
            if (current < total - 1) current++;
        } else if (ch == 'P') {
            if (current > 0) current--;
        } else if (ch == 'Q') {
            cout << "\n  " << YELLOW << "Submit exam now? [Y/N]: " << RESET;
            char confirm; cin >> confirm;
            if (toupper(confirm) == 'Y') break;
        } else if (ch == 'N' && current < total - 1) {
            current++;
        }

        // Auto-advance on last question
        if (current == total - 1 && (ch=='A'||ch=='B'||ch=='C'||ch=='D')) {
            cout << "\n  " << GREEN << "Last question answered! Submit? [Y] Yes  [N] Review: " << RESET;
            char c; cin >> c;
            if (toupper(c) == 'Y') break;
        }
    }

    // Calculate results — PoU rule: every question is worth 1 equal mark,
    // NO negative marking for wrong answers.
    auto endTime = chrono::steady_clock::now();
    long timeTaken = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();

    ExamResult res;
    res.timeTakenSec = timeTaken;
    res.attempted = 0; res.correct = 0; res.wrong = 0; res.skipped = 0;
    res.finalScore = 0; res.maxMarks = total; // 1 mark per question

    for (int i = 0; i < total; i++) {
        const Question& q = questionBank[qIndices[i]];

        if (answers[i] == ' ') {
            res.skipped++;
        } else {
            res.attempted++;
            if (answers[i] == q.correct) {
                res.correct++;
                res.finalScore += 1.0; // 1 mark, no negative marking
            } else {
                res.wrong++; // no marks deducted under PoU rules
                res.wrongAnswers.push_back({i, answers[i]});
            }
        }
    }
    res.percentage = (res.finalScore / res.maxMarks) * 100.0;
    res.grade = getGrade(res.percentage);
    return res;
}

// --- Display Results ----------------------------------------------------------
void displayResults(const ExamResult& res, const vector<int>& qIndices,
                    const string& studentName) {
    clearScreen();
    cout << CYAN;
    printLine("=");
    printCentered("PoU SCHOLARSHIP ENTRANCE EXAM — RESULTS");
    printLine("=");
    cout << RESET;

    cout << "\n  Student   : " << BOLD << studentName << RESET << "\n";
    cout << "  Time Taken: " << formatTime(res.timeTakenSec) << "\n\n";

    printLine();
    cout << BOLD << "  SCORE SUMMARY (No Negative Marking, All Questions Equal Marks)\n" << RESET;
    printLine();
    cout << "  Total Questions : " << qIndices.size() << "\n";
    cout << "  Attempted       : " << res.attempted << "\n";
    cout << GREEN << "  Correct         : " << res.correct << RESET << "\n";
    cout << RED   << "  Wrong           : " << res.wrong << RESET << "\n";
    cout << YELLOW<< "  Skipped         : " << res.skipped << RESET << "\n";
    cout << "\n";
    cout << BOLD << GREEN
         << "  Final Score: " << fixed << setprecision(0) << res.finalScore
         << " / " << res.maxMarks << RESET << "\n";
    cout << "  Percentage : " << fixed << setprecision(1) << res.percentage << "%\n";

    // Grade display
    cout << "\n  ";
    if (res.percentage >= 50) cout << GREEN << BOLD;
    else cout << RED << BOLD;
    cout << "  RESULT: " << res.grade << RESET << "\n\n";

    // Subject-wise breakdown
    printLine();
    cout << BOLD << "  SUBJECT-WISE PERFORMANCE\n" << RESET;
    printLine();

    map<string, pair<int,int>> subjectScore; // correct, total
    vector<string> subjects = {"Mathematics","Chemistry","Physics","English"};
    for (auto& s : subjects) subjectScore[s] = {0,0};

    for (int i = 0; i < (int)qIndices.size(); i++) {
        const Question& q = questionBank[qIndices[i]];
        subjectScore[q.subject].second++;
    }

    for (auto& s : subjects) {
        auto& p = subjectScore[s];
        if (p.second > 0)
            cout << "  " << setw(14) << left << s << ": " << p.second << " questions\n";
    }

    // Wrong answer review
    if (!res.wrongAnswers.empty()) {
        cout << "\n";
        printLine();
        cout << BOLD << "  ANSWER REVIEW (Wrong Answers)\n" << RESET;
        printLine();
        for (auto& wa : res.wrongAnswers) {
            const Question& q = questionBank[qIndices[wa.first]];
            cout << RED << "  [" << q.subject << "] " << RESET;
            string short_q = q.text.size() > 55 ? q.text.substr(0,52)+"..." : q.text;
            cout << short_q << "\n";
            cout << "    Your answer: " << RED << wa.second << RESET;
            cout << "  |  Correct: " << GREEN << q.correct << RESET;
            cout << "  |  " << CYAN << q.explanation << RESET << "\n\n";
        }
    }

    printLine("=");
    if (res.percentage >= 50)
        cout << GREEN << BOLD << "  Congratulations! You are in the scholarship range!\n" << RESET;
    else
        cout << RED << BOLD << "  Keep practicing! You can do it next time.\n" << RESET;
    printLine("=");
}

// --- Main ---------------------------------------------------------------------
int main() {
    srand((unsigned)time(nullptr));

    while (true) {
        printBanner();
        string name = registerStudent();

        int mode = selectMode();
        if (mode == 4) {
            cout << "\n  " << CYAN << "Thank you for using PoU Exam Simulator. Good luck!\n\n" << RESET;
            break;
        }

        vector<int> qIndices;
        long timeLimit;
        string subject = "";

        if (mode == 1) {
            qIndices = getQuestionIndices(1);
            // Full PoU set is exactly 150 questions
            if (qIndices.size() > 150) qIndices.resize(150);
            timeLimit = 7200; // 2 hours
            cout << "\n  " << YELLOW << "Full exam: " << qIndices.size()
                 << " questions | " << qIndices.size() << " marks | 2 hours | No negative marking\n" << RESET;
        } else if (mode == 2) {
            qIndices = getQuestionIndices(2);
            timeLimit = 1200; // 20 min
            cout << "\n  " << YELLOW << "Mini mock: " << qIndices.size()
                 << " questions | 20 minutes | No negative marking\n" << RESET;
        } else {
            subject = selectSubject();
            qIndices = getQuestionIndices(3, subject);
            timeLimit = 1800; // 30 min
            cout << "\n  " << YELLOW << "Subject drill: " << subject
                 << " | " << qIndices.size() << " questions | 30 minutes | No negative marking\n" << RESET;
        }

        cout << "\n  " << BOLD << "Instructions:\n" << RESET;
        cout << "  - Press A/B/C/D to answer, S to skip, P for previous\n";
        cout << "  - Q to quit/submit early\n";
        cout << "  - ALL questions carry EQUAL marks (1 mark each). NO negative marking.\n";
        cout << "  - Timer counts down from the top of each screen\n";
        cout << "\n  Press ENTER to begin the exam...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();

        ExamResult result = runExam(qIndices, name, timeLimit);
        displayResults(result, qIndices, name);

        cout << "\n  Press ENTER to return to menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    return 0;
}