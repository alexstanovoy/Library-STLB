#include <string>
#include <iostream>
#include "fixed_uint.hpp"

template<typename T>
T fib(const size_t n) {
    T a = 0, b = 1, c;
    if (n == 0) {
        return a;
    }
    if (n == 1) {
        return b;
    }
    for (size_t i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main() {
    const std::string ans1 = "1347615707148841369192204067953771473429643549238341113781505897799744508412240184909438753863984547433408572552053041297363539128565227038245446402778857";
    const std::string ans2 = "16838932431231352876383";
    const std::string ans3 = "70413756354373050725769392385805528387679142265209363923365877765354801731004148306529740697062006433520377153408388494268707123703038129765393227866694676475128013234554964144501969020991005210274801723053680698469485605582989035394857993480813664022960065973379289264415134890050740883471597468862385996422901198203153019996132098612847368540183640023039200800947725426908306055877300201470555475389830515267091043234551613612740401881136115267507560108247717794850800492415455553422554930473940375515173064239712062833169153424434260441930364750118238019463707644654500189015044091912097253839499559044382477090372525446434147908333466298477272997285404267599782931129798768595293399787817364632361422759374716835820957440105460001491733665150658125463316864172086057419546487230534607206120945194488530592780019956710298235224153938828675453947094030767832038922457481127701900192613896508113287024935933115270264677629005778257267087723365344313205741324082043932379809295830447137082204168551659939396185119444608944209189258073439708112309492577990685334535945802614835199142509652350285712965858349210006542149973598045425406664742104663156819538780104688347958860248790451784558070041894640994478111727454470609656117459520176717073112242507494400599968571907470107916846616092543310997297750815781126216527380796441305515708440685415769500887618152490316921";
    exit(
        fib<stlb::fixed_uint<512>>(134534).to_string() != ans1 ||
        fib<stlb::fixed_uint<75>>(13452334).to_string() != ans2 ||
        fib<stlb::fixed_uint<12832>>(6542).to_string() != ans3
    );
}