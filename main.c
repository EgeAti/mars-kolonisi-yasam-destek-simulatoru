/* Mars Kolonisi Yasam Destek Simulatoru - Algoritma ve Programlama Odev 1
 * (Ekran ciktilari konsolda sorun cikmasin diye ASCII yazilmistir.) */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int saglik = 100, enerji = 100, oksijen = 100, su, parca, hasar = 0;
int kilitMuhurlu = 0, gun = 1, tur = 0;
int firtinaTur, firtinaHasar, esikYuksek, esikDusuk;   /* zorluga gore */

void temizle(void) { int c; while ((c = getchar()) != '\n' && c != EOF); }

/* Degerleri 0-100 araliginda tutar (her komuttan sonra cagrilir) */
void sinirla(void) {
    if (saglik < 0) saglik = 0;
    if (saglik > 100) saglik = 100;
    if (enerji < 0) enerji = 0;
    if (enerji > 100) enerji = 100;
    if (oksijen < 0) oksijen = 0;
    if (oksijen > 100) oksijen = 100;
    if (hasar < 0) hasar = 0;
    if (hasar > 100) hasar = 100;
    if (su < 0) su = 0;
    if (parca < 0) parca = 0;
}

/* Enerji yetersizse komutu iptal ettirir (1 dondurur) */
int enerjiYok(int gereken) {
    if (enerji < gereken) { printf("Enerji yetersiz (en az %d). Komut iptal edildi.\n", gereken); return 1; }
    return 0;
}

void zorlukSec(void) {
    char z; int gecerli;
    do {
        gecerli = 1;
        printf("Zorluk seviyesi (K: Kolay, N: Normal, Z: Zor): ");
        scanf(" %c", &z); temizle();
        switch (z) {
            case 'k': case 'K': su = 30; parca = 3; firtinaTur = 3; firtinaHasar = 5;  esikYuksek = 50; esikDusuk = 30; break;
            case 'n': case 'N': su = 20; parca = 2; firtinaTur = 5; firtinaHasar = 8;  esikYuksek = 40; esikDusuk = 20; break;
            case 'z': case 'Z': su = 10; parca = 1; firtinaTur = 7; firtinaHasar = 12; esikYuksek = 30; esikDusuk = 15; break;
            default: printf("UYARI: Gecersiz zorluk seviyesi!\n"); gecerli = 0;
        }
    } while (!gecerli);
}

/* G: Kesif */
int kesif(void) {
    int zar, esik;
    if (enerjiYok(15)) return 0;
    enerji -= 15; oksijen -= 10;
    if (enerji > 40 && oksijen > 30) esik = esikYuksek; else esik = esikDusuk;
    zar = rand() % 100;
    printf("Yaralanma -> Zar: %d / Esik: 10 (hasar>70 ise kesin)\n", zar);
    if (zar < 10 || hasar > 70) { saglik -= 10; printf("Yaralandin! Saglik -10.\n"); }
    zar = rand() % 100;
    printf("Kesif -> Zar: %d / Esik: %d -> ", zar, esik);
    if (zar < esik)           { su += 10;   printf("BASARILI: 10 su buldun.\n"); }
    else if (zar < esik + 20) { parca += 1; printf("BASARILI: 1 yedek parca buldun.\n"); }
    else                      printf("BASARISIZ: Eli bos dondun.\n");
    return 1;
}

/* O: Oksijen uretimi (2 su -> 3 oksijen) */
int oksijenUret(void) {
    int miktar, artik;
    if (enerjiYok(10)) return 0;
    printf("Kac birim su harcanacak? (mevcut: %d): ", su);
    if (scanf("%d", &miktar) != 1) miktar = 0;
    temizle();
    if (miktar < 2 || miktar > su) { printf("Gecersiz miktar! Iptal edildi.\n"); return 0; }
    artik = miktar % 2;                       /* tekse 1 birim artik kalir */
    oksijen += ((miktar - artik) / 2) * 3;
    su -= miktar - artik;
    enerji -= 10;
    printf("%d su harcandi, %d oksijen uretildi. Artik su: %d\n", miktar - artik, ((miktar - artik) / 2) * 3, artik);
    return 1;
}

/* T: Tamir (hasar kademesine gore IF - ELSE IF - ELSE) */
int tamir(void) {
    int gereken, onarim;
    if (parca < 1) { printf("Yedek parca yok! Tamir yapilamaz.\n"); return 0; }
    if (hasar == 0) { printf("Hasar yok, tamire gerek yok.\n"); return 0; }
    if (enerjiYok(10)) return 0;
    if (hasar <= 30)      { gereken = 1; onarim = 20; printf("Hafif hasar: "); }
    else if (hasar <= 70) { gereken = 2; onarim = 35; printf("Orta hasar: "); }
    else                  { gereken = 3; onarim = 60; printf("Agir hasar: "); }
    if (parca < gereken) { printf("%d parca gerekli, %d var. Iptal.\n", gereken, parca); return 0; }
    parca -= gereken; hasar -= onarim; enerji -= 10;
    printf("%d parca harcandi, %d hasar onarildi.\n", gereken, onarim);
    return 1;
}

/* D: Durum raporu (tur harcamaz) */
void durumRaporu(void) {
    printf("\n--- DURUM --- Gun:%d Tur:%d\nSaglik:%d Enerji:%d Oksijen:%d Su:%d Parca:%d Hasar:%d Kilit:%d\n",
           gun, tur, saglik, enerji, oksijen, su, parca, hasar, kilitMuhurlu);
    if (oksijen < 25) printf("UYARI: Oksijen kritik!\n");
    if (saglik < 30)  printf("UYARI: Saglik kritik!\n");
    if (hasar > 70)   printf("UYARI: Modul hasari kritik!\n");
    if (enerji < 20)  printf("UYARI: Enerji kritik!\n");
    if (oksijen >= 25 && saglik >= 30 && hasar <= 70 && enerji >= 20) printf("TUM SISTEMLER NORMAL\n");
    printf("\n");
}

/* U: Dinlen */
int dinlen(void) {
    enerji += 25;
    if (oksijen < 20) {                       /* ic ice IF */
        if (enerji > 50) { saglik -= 5;  printf("Oksijen dusuk! Saglik -5.\n"); }
        else             { saglik -= 10; printf("Oksijen dusuk ve bitkinsin! Saglik -10.\n"); }
    } else { saglik += 10; printf("Dinlendin: Enerji +25, Saglik +10.\n"); }
    return 1;
}

/* K: Kum firtinasi (FOR, break, continue) */
int firtina(void) {
    int i, zar, carpan, esik, hasarTur = 0, bs = saglik, bo = oksijen;
    if (enerjiYok(10)) return 0;
    enerji -= 10;
    printf("\n*** KUM FIRTINASI (%d tur) ***\n", firtinaTur);
    for (i = 1; i <= firtinaTur; i++) {
        carpan = (i % 2 == 0) ? 2 : 1;        /* cift turda hasar 2 kat */
        esik = (saglik < 40) ? 35 : 25;       /* zayif oyuncu daha cok zarar gorur */
        zar = rand() % 100;
        printf("Tur %d%s | Zar: %d / ", i, carpan == 2 ? " (SIDDETLI)" : "", zar);
        if (zar < esik) {
            saglik -= firtinaHasar * carpan; hasarTur++;
            printf("Esik: %d -> CAN KAYBI (-%d)\n", esik, firtinaHasar * carpan);
        } else if (zar < esik + 20) {
            if (kilitMuhurlu == 1) { printf("Esik: %d -> Sizinti, kilit muhurlu: ATLANDI\n", esik + 20); continue; }
            oksijen -= firtinaHasar * carpan; hasarTur++;
            printf("Esik: %d -> OKSIJEN SIZINTISI (-%d)\n", esik + 20, firtinaHasar * carpan);
        } else if (zar < esik + 40) {
            hasar += firtinaHasar * carpan; hasarTur++;
            printf("Esik: %d -> MODUL HASARI (+%d)\n", esik + 40, firtinaHasar * carpan);
        } else printf("Esik: %d -> Zararsiz korundun.\n", esik + 40);
        sinirla();
        if (saglik == 0 || oksijen == 0) { printf("Kritik seviye! Firtina durdu.\n"); break; }
    }
    printf("--- Ozet: %d turda hasar | Saglik kaybi: %d | Oksijen kaybi: %d ---\n", hasarTur, bs - saglik, bo - oksijen);
    return 1;
}

/* H: Hava kilidi sifresi (DO-WHILE, 3 deneme) */
int havaKilidi(void) {
    char giris, sifre = 'A' + (gun % 26);     /* sifre gune bagli */
    int deneme = 0;
    if (kilitMuhurlu == 1) { printf("Hava kilidi zaten muhurlu.\n"); return 0; }
    if (enerjiYok(15)) return 0;
    printf("Ipucu: Sifre, alfabede A harfinden bugunun gun sayisi kadar ilerideki harftir.\n");
    do {
        printf("Sifre (deneme %d/3): ", deneme + 1);
        scanf(" %c", &giris); temizle();
        giris = toupper(giris);               /* kucuk harf de kabul */
        if (giris < 'A' || giris > 'Z') { printf("Sadece A-Z girin (hak harcanmadi).\n"); continue; }
        if (giris != sifre) {
            deneme++; enerji -= 5;
            printf(giris < sifre ? "Yanlis! Ileri git. (Enerji -5)\n" : "Yanlis! Geri git. (Enerji -5)\n");
        }
    } while (giris != sifre && deneme < 3);
    if (giris == sifre) { kilitMuhurlu = 1; printf("Dogru! Hava kilidi muhurlendi.\n"); }
    else                { hasar += 20;      printf("3 deneme basarisiz! Modul hasari +20.\n"); }
    return 1;
}

int main(void) {
    char komut;
    int turSay, hayattaGun, sonuc = 0;        /* sonuc: 0 devam, 1 kazandi, 2 kaybetti, 3 cikis */
    srand(time(NULL));
    printf("=== MARS KOLONISI YASAM DESTEK SIMULATORU ===\n15 gun hayatta kal!\n\n");
    zorlukSec();
    printf("\nKomutlar: G:Kesif O:Oksijen T:Tamir D:Durum U:Uyu K:Kum firtinasi H:Hava kilidi Q:Cikis\n\n");

    do {
        printf("Komut: ");
        scanf(" %c", &komut); temizle();
        turSay = 0;
        switch (komut) {
            case 'g': case 'G': turSay = kesif();       break;
            case 'o': case 'O': turSay = oksijenUret(); break;
            case 't': case 'T': turSay = tamir();       break;
            case 'd': case 'D': durumRaporu();          break;
            case 'u': case 'U': turSay = dinlen();      break;
            case 'k': case 'K': turSay = firtina();     break;
            case 'h': case 'H': turSay = havaKilidi();  break;
            case 'q': case 'Q': sonuc = 3;              break;
            default: printf("UYARI: Tanimsiz komut! (Tur harcanmadi)\n");
        }
        if (turSay == 1) {
            tur++;
            if (tur % 3 == 0) {               /* her 3 turda yeni gun */
                gun++; oksijen -= 8;
                printf(">>> YENI GUN: %d | Oksijen -8", gun);
                if (hasar > 70) { saglik -= 5; printf(" | Hasar yuksek: Saglik -5"); }
                printf("\n");
            }
        }
        sinirla();
        if (turSay == 1)
            printf("[Gun %d | Tur %d] Saglik:%d Enerji:%d Oksijen:%d Su:%d Parca:%d Hasar:%d\n",
                   gun, tur, saglik, enerji, oksijen, su, parca, hasar);
        if (saglik == 0 || oksijen == 0) sonuc = 2;
        else if (gun > 15) sonuc = 1;
    } while (sonuc == 0);

    /* Sonuc raporu */
    hayattaGun = gun - 1;                     /* tamamlanan gun */
    printf("\n========== SONUC RAPORU ==========\n");
    if (sonuc == 1)      printf("KAZANDIN! Kurtarma ekibi geldi.\n");
    else if (sonuc == 2) printf("KAYBETTIN! Hayatta kalamadin.\n");
    else                 printf("Oyundan cikildi.\n");
    printf("Hayatta kalinan gun: %d | Toplam tur: %d\n", hayattaGun, tur);
    printf("Saglik:%d Enerji:%d Oksijen:%d Su:%d Parca:%d Hasar:%d\n", saglik, enerji, oksijen, su, parca, hasar);
    printf("PUAN: %d (gun*100 + saglik + oksijen - hasar)\n", hayattaGun * 100 + saglik + oksijen - hasar);
    return 0;
}
