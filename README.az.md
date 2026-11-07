<div align="center">
  <img src="assets/thread-logo.svg" width="132" alt="THREAD OS loqosu" />
  <h1>THREAD OS</h1>
  <p><strong>Sənin kompüterin. Sənin qaydaların. Sənin sistemin.</strong></p>
  <p>Azadlıq, fərdiləşdirmə, məhsuldarlıq və istifadəçi nəzarəti üçün Linux əsaslı x86_64 əməliyyat sistemi layihəsi.</p>
  <a href="README.md"><img alt="English" src="https://img.shields.io/badge/🇬🇧_English-0B0D12?style=for-the-badge"></a>
  <a href="README.ru.md"><img alt="Русский" src="https://img.shields.io/badge/🇷🇺_Русский-0B0D12?style=for-the-badge"></a>
  <a href="README.de.md"><img alt="Deutsch" src="https://img.shields.io/badge/🇩🇪_Deutsch-0B0D12?style=for-the-badge"></a>
  <a href="README.az.md"><img alt="Azərbaycan dili" src="https://img.shields.io/badge/🇦🇿_Azərbaycanca-0B0D12?style=for-the-badge"></a>
  <br><br>
  <a href="ARCHITECTURE.md"><img alt="Arxitektura" src="https://img.shields.io/badge/arxitektura-Linux%20%2B%20THREAD-8AE3C5?style=for-the-badge"></a>
  <a href="ROADMAP.md"><img alt="Layihənin vəziyyəti" src="https://img.shields.io/badge/vəziyyət-ilkin%20inkişaf-7BB7FF?style=for-the-badge"></a>
</div>

---

## ✦ THREAD-ə xoş gəlmisiniz

THREAD OS **Cəmil İslamovun** əməliyyat sistemi layihəsidir. İlk buraxılışın **Linux nüvəsi** və **GRUB** əsasında qurulması planlaşdırılır. THREAD kimliyi xüsusi iş masası, tətbiqlər, iş sahəsi alətləri və quraşdırıcı ilə yaradılacaq.

> **Layihənin real vəziyyəti:** bu repozitoriya ilkin inkişaf təməlidir, hazır əməliyyat sistemi deyil. Yoxlanılmış yüklənə bilən quraşdırma ISO-su və sınaqdan keçirilmiş iş masası hələ yoxdur. Mövcud ISO skripti repozitoriyada olmayan nüvə və initramfs fayllarını tələb edir. QEMU-da və ya fiziki kompüterdə yükləmə sınağı aparılmayıb.

## ✧ Sürətli keçidlər

[⚙ **Qurulma təlimatı**](BUILD.md) · [📦 **Quraşdırma təlimatı**](INSTALL.md) · [🧭 **Arxitektura**](ARCHITECTURE.md) · [🗺 **Yol xəritəsi**](ROADMAP.md) · [🤝 **Töhfə vermək**](CONTRIBUTING.md) · [☷ **Məsələlər**](https://github.com/jamilislamov642-cell/thread-os/issues) · [🌐 **Veb-sayt**](https://jynza.cc)

## ◈ Layihənin vəziyyəti

**Mərhələ: ilkin təməl.** Repozitoriyada sənədlər, ilkin GRUB konfiqurasiyası, brend materialları və başlanğıc skriptləri var. Bunlar işlək OS təsvirinin mövcud olduğunu göstərmir: hazırkı qurma skripti tam live sistemi yaratmır; nüvə/initramfs və düzgün live-boot davranışı hələ əlavə edilməlidir.

| Sahə | Vəziyyət |
|:--|:--|
| Linux əsaslı x86_64 arxitekturası | Planlaşdırılıb və sənədləşdirilib |
| GRUB BIOS + UEFI yüklənməsi | Konfiqurasiya başlayıb; yükləmə sınağı yoxdur |
| Nüvə və initramfs | Daxil edilməyib |
| Tam live kök fayl sistemi | Hazır deyil |
| THREAD iş masası və işlək tətbiqlər | Planlaşdırılıb; tam iş masası hazırlanmayıb |
| Qrafik quraşdırıcı və diskə quraşdırma | Hazır deyil |
| Yoxlanılmış `THREAD.iso` | Hələ mövcud deyil |
| QEMU / fiziki avadanlıq sınaqları | Aparılmayıb |

Yol xəritəsindəki xüsusiyyətlər məqsəddir, artıq işləyən funksiyalar barədə iddia deyil. Vəziyyət yalnız həyata keçirilmə və sınaq nəticələrinə əsasən dəyişdiriləcək.

## ⌘ Texniki istiqamət

İlk hədəf **x86_64**, Linux nüvəsi və Legacy BIOS/UEFI üçün GRUB-dur. THREAD-in fərqli kimliyi müstəqil nüvə barədə əsassız iddia ilə deyil, öz iş masası, tətbiqləri, parametrləri, iş sahəsi funksiyaları, dizaynı və quraşdırma vasitələri ilə formalaşacaq.

Planlaşdırılan sahələr:

- **Yükləmə:** GRUB, BIOS/UEFI, bərpa və təhlükəsiz qrafika rejimi.
- **Sistem:** Linux drayverləri, cihaz aşkarlanması, yaddaş qurğuları, şəbəkə, hesablar və jurnallar.
- **İş masası:** panel, tətbiq başladıcısı, bildirişlər və parametrlər olan tünd THREAD interfeysi.
- **Tətbiqlər:** Files, Settings, Terminal, Browser, Store, Notes, Monitor, Network, Security, Update Center, Recovery və Workspace.
- **Quraşdırıcı:** diskin aydın seçimi, əl ilə və ya avtomatik bölmələmə, təhlükəli əməliyyatdan əvvəl təsdiq.

Ətraflı məlumat: [arxitektura](ARCHITECTURE.md) və [yol xəritəsi](ROADMAP.md).

## 🛠 Qurma və sınaq

Mövcud skriptlər **ilkin alətlərdir**, hazır ISO qurma sistemi deyil. Yarana biləcək təsviri kompüterə quraşdırmaq və ya USB-ni silmək üçün istifadə etməyin. Nüvə və kök fayl sistemi inteqrasiyası, live yükləmə, quraşdırıcı və yükləmə sınaqları hələ tamamlanmayıb.

[BUILD.md](BUILD.md) sənədinə baxın. Mövcud skriptlər:

```bash
./scripts/build_iso.sh
./scripts/validate_iso.sh
./scripts/qemu_test.sh
```

Bu əmrləri işə salmaq OS-in yükləndiyini **sübut etmir**. Buraxılış yalnız tam sistem təsviri yaradıldıqdan və BIOS/UEFI sınaqları aparıldıqdan sonra yüklənə bilən adlandırıla bilər.

## 🗂 Repozitoriyanın quruluşu

`bootloader/` GRUB · `kernel/` Linux nüvəsi · `desktop/` iş masası · `applications/` tətbiqlər · `system/` xidmətlər · `drivers/` drayverlər · `installer/` quraşdırıcı · `filesystem/` fayl sistemi · `networking/` şəbəkə · `security/` təhlükəsizlik · `themes/` mövzular · `assets/` qrafika · `scripts/` skriptlər · `tests/` sınaqlar · `documentation/` sənədlər · `iso/` ISO qeydləri.

## 🧵 Layihə prinsipləri

1. **Görünüşdən əvvəl işlək funksiya:** maket işlək sistem kimi təqdim edilmir.
2. **Dürüstlük:** yalnız həyata keçirilən və yoxlanılan xüsusiyyətlər işlək sayılır.
3. **İstifadəçi nəzarəti:** riskli əməliyyatlar açıq razılıq tələb edir.
4. **Davamlı texniki təməl:** əvvəl Linux komponentləri, sonra mümkün nüvə müstəqilliyi.
5. **Fərdiləşdirmə:** istifadəçi görünüş və davranışı idarə etməlidir.

## 🤝 Töhfə verin

[CONTRIBUTING.md](CONTRIBUTING.md) sənədini oxuyun, [açıq məsələlərə](https://github.com/jamilislamov642-cell/thread-os/issues) baxın və ya [yeni məsələ yaradın](https://github.com/jamilislamov642-cell/thread-os/issues/new/choose). Sınaq mühitini və dəqiq nəticələri qeyd edin.

**THREAD OS** · Təsisçi: **Cəmil İslamov** · [jynza.cc](https://jynza.cc) · [MIT lisenziyası](LICENSE)

<div align="center"><a href="#-thread-ə-xoş-gəlmisiniz">↑ Yuxarı</a></div>
