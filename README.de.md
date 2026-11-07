<div align="center">
  <img src="assets/thread-logo.svg" width="132" alt="THREAD OS-Logo" />
  <h1>THREAD OS</h1>
  <p><strong>Dein Computer. Deine Regeln. Dein System.</strong></p>
  <p>Ein Linux-basiertes x86_64-Betriebssystemprojekt für Freiheit, Anpassbarkeit, Produktivität und Nutzerkontrolle.</p>
  <a href="README.md"><img alt="English" src="https://img.shields.io/badge/🇬🇧_English-0B0D12?style=for-the-badge"></a>
  <a href="README.ru.md"><img alt="Русский" src="https://img.shields.io/badge/🇷🇺_Русский-0B0D12?style=for-the-badge"></a>
  <a href="README.de.md"><img alt="Deutsch" src="https://img.shields.io/badge/🇩🇪_Deutsch-0B0D12?style=for-the-badge"></a>
  <a href="README.az.md"><img alt="Azərbaycan dili" src="https://img.shields.io/badge/🇦🇿_Azərbaycanca-0B0D12?style=for-the-badge"></a>
  <br><br>
  <a href="ARCHITECTURE.md"><img alt="Architektur" src="https://img.shields.io/badge/architektur-Linux%20%2B%20THREAD-8AE3C5?style=for-the-badge"></a>
  <a href="ROADMAP.md"><img alt="Projektstatus" src="https://img.shields.io/badge/status-frühe%20Entwicklung-7BB7FF?style=for-the-badge"></a>
</div>

---

## ✦ Willkommen bei THREAD

THREAD OS ist ein Betriebssystemprojekt von **Jamil Islamov**. Die erste Version ist mit dem **Linux-Kernel** und **GRUB** geplant. Die THREAD-Identität soll durch einen eigenen Desktop, Anwendungen, Workspace-Werkzeuge und einen Installer entstehen.

> **Ehrlicher Projektstatus:** Dieses Repository ist eine frühe Entwicklungsbasis, kein fertiges Betriebssystem. Ein verifiziertes bootfähiges Installationsmedium oder ein getesteter Desktop sind noch nicht verfügbar. Das aktuelle ISO-Skript benötigt Kernel- und initramfs-Dateien, die noch nicht enthalten sind. Boottests in QEMU oder auf physischer Hardware wurden nicht durchgeführt.

## ✧ Schnellzugriff

[⚙ **Build-Anleitung**](BUILD.md) · [📦 **Installation**](INSTALL.md) · [🧭 **Architektur**](ARCHITECTURE.md) · [🗺 **Roadmap**](ROADMAP.md) · [🤝 **Mitwirken**](CONTRIBUTING.md) · [☷ **Issues**](https://github.com/jamilislamov642-cell/thread-os/issues) · [🌐 **Website**](https://jynza.cc)

## ◈ Projektstatus

**Phase: frühe Grundlage.** Das Repository enthält Dokumentation, eine vorläufige GRUB-Konfiguration, Branding und erste Skripte. Das ist kein Nachweis für ein funktionierendes OS-Image: Das aktuelle Build-Skript erstellt noch kein vollständiges Live-System; Kernel/initramfs und korrektes Live-Boot-Verhalten fehlen.

| Bereich | Status |
|:--|:--|
| Linux-basierte x86_64-Architektur | Geplant und dokumentiert |
| GRUB-BIOS- und UEFI-Boot | Konfiguration begonnen; nicht bootgetestet |
| Kernel und initramfs | Nicht enthalten |
| Vollständiges Live-Root-Dateisystem | Nicht implementiert |
| THREAD-Desktop und funktionierende Apps | Geplant; kein vollständiger Desktop implementiert |
| Grafischer Installer und Festplatteninstallation | Nicht implementiert |
| Verifiziertes `THREAD.iso` | Noch nicht verfügbar |
| QEMU-/Hardware-Tests | Nicht durchgeführt |

Roadmap-Einträge sind Ziele, keine Behauptungen über bereits funktionierende Features. Der Status wird anhand von Implementierung und Tests aktualisiert.

## ⌘ Technische Ausrichtung

Das erste Ziel ist **x86_64** mit Linux-Kernel und GRUB für Legacy-BIOS und UEFI. Die THREAD-Identität soll aus Desktop, Anwendungen, Einstellungen, Workspace-Funktionen, Design und Installationswerkzeugen kommen—not aus der falschen Behauptung eines unabhängig entwickelten Kernels.

Geplante Bereiche:

- **Boot:** GRUB, BIOS/UEFI, Wiederherstellung und sicherer Grafikmodus.
- **System:** Linux-Treiber, Geräteerkennung, Speicher, Netzwerk, Konten und Protokolle.
- **Desktop:** dunkle THREAD-Oberfläche mit Panel, App-Launcher, Einstellungen und Benachrichtigungen.
- **Apps:** Files, Settings, Terminal, Browser, Store, Notes, Monitor, Network, Security, Update Center, Recovery und Workspace.
- **Installer:** eindeutige Laufwerksauswahl, manuelle/geführte Partitionierung und Bestätigung vor destruktiven Aktionen.

Mehr dazu: [Architektur](ARCHITECTURE.md) und [Roadmap](ROADMAP.md).

## 🛠 Build und Tests

Die aktuellen Skripte sind **erste Werkzeuge**, keine fertige ISO-Build-Pipeline. Ein möglicherweise erzeugtes Image darf nicht zur Installation oder zum Überschreiben eines USB-Laufwerks verwendet werden. Kernel-/Root-Dateisystem-Integration, Live-Boot, Installer und Boottests fehlen noch.

Siehe [BUILD.md](BUILD.md). Aktuelle Skript-Einstiegspunkte:

```bash
./scripts/build_iso.sh
./scripts/validate_iso.sh
./scripts/qemu_test.sh
```

Diese Befehle beweisen **nicht**, dass das OS bootet. Als bootfähig darf ein Release erst nach vollständiger Image-Erstellung und BIOS-/UEFI-Tests in einer Testumgebung bezeichnet werden.

## 🗂 Repository-Übersicht

`bootloader/` GRUB · `kernel/` Linux-Kernel · `desktop/` Desktop · `applications/` Apps · `system/` Dienste · `drivers/` Treiber · `installer/` Installer · `filesystem/` Dateisystem · `networking/` Netzwerk · `security/` Sicherheit · `themes/` Themes · `assets/` Grafiken · `scripts/` Skripte · `tests/` Tests · `documentation/` Doku · `iso/` ISO-Hinweise.

## 🧵 Grundsätze

1. **Funktion statt Fassade:** Ein Mockup wird nicht als OS-Funktion ausgegeben.
2. **Ehrliche Aussagen:** Nur implementierte und getestete Funktionen als solche bezeichnen.
3. **Nutzerkontrolle:** Riskante Aktionen benötigen eine ausdrückliche Zustimmung.
4. **Wartbare Basis:** Erst etablierte Linux-Komponenten, später mögliche Kernel-Unabhängigkeit.
5. **Anpassbarkeit:** Nutzer sollen Aussehen und Verhalten kontrollieren können.

## 🤝 Mitwirken

Lies [CONTRIBUTING.md](CONTRIBUTING.md), prüfe [offene Issues](https://github.com/jamilislamov642-cell/thread-os/issues) oder [erstelle ein Issue](https://github.com/jamilislamov642-cell/thread-os/issues/new/choose). Bitte nenne Testumgebung und konkrete Ergebnisse.

**THREAD OS** · Gründer: **Jamil Islamov** · [jynza.cc](https://jynza.cc) · [MIT-Lizenz](LICENSE)

<div align="center"><a href="#-willkommen-bei-thread">↑ Nach oben</a></div>
