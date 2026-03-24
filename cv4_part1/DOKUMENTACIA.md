# 📚 Dokumentácia Projektu - 2D Simulátor Robota (Časť 1)

**Projekt:** Simulované Prostredie s LIDAR Senzorom  
**Jazyk:** C++17  
**Build Systém:** CMake  
**Testy:** Google Test Framework  
**Vizualizácia:** OpenCV

---

## 📁 Štruktúra Súborov

```
cv4_part1/
├── include/
│   ├── Geometry.h           ← Základné dátové štruktúry
│   ├── Environment.h        ← Mapa prostredia
│   ├── Lidar.h              ← LIDAR senzor
│   └── Canvas.h             ← Vizualizácia
├── src/
│   ├── main.cpp             ← Demo aplikácia
│   ├── Environment.cpp
│   ├── Lidar.cpp
│   └── Canvas.cpp
├── tests/
│   └── robot_test.cpp       ← Unit testy (11 testov)
├── CMakeLists.txt           ← Build konfigurácia
└── opk-map.png              ← PNG mapa
```

---

# 🔧 Podrobný Popis Všetkých Súborov

---

## 1️⃣ CMakeLists.txt - Build Konfigurácia

### Úvod a Verzie
```cmake
cmake_minimum_required(VERSION 3.10)
project(cv4_part1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```
**Čo to robí:**
- `cmake_minimum_required` - požaduje CMake 3.10+
- `project(cv4_part1)` - názov projektu
- `CMAKE_CXX_STANDARD 17` - použije sa C++17

---

### OpenCV Knižnica
```cmake
find_package(OpenCV REQUIRED)
```
**Čo to robí:** Nájde a pripojaí OpenCV knižnicu (na kreslenie a PNG)

---

### Google Test Framework
```cmake
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)
```
**Čo to robí:**
- `FetchContent` - automaticky stiahne knižnicu z GitHub-u
- URL - stiahne konkrétnu verziu (v1.14.0)
- `MakeAvailable` - sprístupní knižnicu v projekte

---

### Demo Aplikácia (cv4_part1)
```cmake
add_executable(cv4_part1
    src/main.cpp
    src/Canvas.cpp
    src/Environment.cpp
    src/Lidar.cpp
)

target_include_directories(cv4_part1 PRIVATE include)
target_link_libraries(cv4_part1 PRIVATE ${OpenCV_LIBS})
```
**Čo to robí:**
- `add_executable` - vytvorí spustiteľný program `cv4_part1`
- Linkuje 4 `.cpp` súbory
- `target_include_directories` - nastaví `include/` priečinok
- `target_link_libraries` - pripojí OpenCV

**Výsledok:** Spustiteľný binárny súbor `./cv4_part1`

---

### Unit Testy (cv4_tests)
```cmake
enable_testing()

add_executable(cv4_tests
    tests/robot_test.cpp
    src/Environment.cpp
    src/Lidar.cpp
)

target_include_directories(cv4_tests PRIVATE include)
target_link_libraries(cv4_tests PRIVATE ${OpenCV_LIBS} GTest::gtest GTest::gtest_main)

add_test(NAME cv4_tests COMMAND cv4_tests)
```
**Čo to robí:**
- `enable_testing()` - aktivuje testovací systém
- `add_executable(cv4_tests` - vytvorí program s testami
- Linkuje Environment a Lidar (aby sa dali testovať)
- Linkuje Google Test knižnice (`gtest`, `gtest_main`)
- `add_test()` - registruje testy pre `ctest`

**Výsledok:** Spustiteľný binárny súbor `./cv4_tests`

---

## 2️⃣ Geometry.h - Základné Dátové Štruktúry

### Point2d - 2D Bod
```cpp
namespace geometry {

struct Point2d {
    double x;
    double y;
};
```
**Čo to je:** Reprezentuje bod v 2D rovine  
**Kde sa používa:** Lidar hit body, pozície  
**Príklad:** `Point2d{5.0, 3.5}` = bod na (5m, 3.5m)

---

### Twist - Rýchlosť
```cpp
struct Twist {
    double linear;      // Lineárna rýchlosť (m/s)
    double angular;     // Uhlová rýchlosť (rad/s)
};
```
**Čo to je:** Kombinovaná rýchlosť robota  
- **linear** - rýchlosť vpred/vzad
- **angular** - rýchlosť rotácie

**Kde sa používa:** RobotState  
**Príklad:** `Twist{1.0, 0.5}` = 1m/s vpred + 0.5 rad/s otáčka

---

### RobotState - Úplný Stav Robota
```cpp
struct RobotState {
    double x;           // Pozícia X (metre)
    double y;           // Pozícia Y (metre)
    double theta;       // Natočenie (radiány, 0 = smerom vpravo)
    Twist velocity;     // Rýchlosť {linear, angular}
};
```
**Čo to je:** Kompletný popis stavu robota v priestore  
**Kde sa používa:** `lidar.scan()` na vstupe  
**Príklad:**
```cpp
RobotState state;
state.x = 5.0;           // Robot na X = 5m
state.y = 3.0;           // Robot na Y = 3m
state.theta = 0.0;       // Natočený na 0° (smerom vpravo)
state.velocity = {0.0, 0.0};  // Nehybný
```

---

## 3️⃣ Environment.h - Deklarácia Mapy

### Config Štruktúra
```cpp
namespace environment {

struct Config {
    std::string map_filename;  // Cesta k PNG súboru
    double resolution;         // Metre na pixel (0.05 = 5cm)
};
```
**Čo to je:** Konfigurácia pre načítanie mapy  
**Ako sa používa:**
```cpp
environment::Config config;
config.map_filename = "../opk-map.png";
config.resolution = 0.05;
```

---

### Environment Trieda
```cpp
class Environment {
public:
    explicit Environment(const Config& config);
    
    bool isOccupied(double x, double y) const;
    double getWidth() const;
    double getHeight() const;

private:
    double resolution_;
    cv::Mat map_;  // OpenCV mapa
};
```

**Metódy:**

#### Konštruktor
```cpp
explicit Environment(const Config& config);
```
- Načíta PNG súbor do `map_`
- Kontroluje či je `resolution > 0`
- Kontroluje či súbor existuje
- Vyhadzuje exceptions ak je chyba

---

#### isOccupied(x, y)
```cpp
bool isOccupied(double x, double y) const;
```
**Čo to robí:** Skúša či pozícia (x, y) je prekážka  
**Vracia:** `TRUE` = prekážka, `FALSE` = voľné miesto  
**Prípad 1 - Negative súradnice:**
```cpp
env.isOccupied(-1.0, -1.0)  // → TRUE (mimo mapy)
```

**Prípad 2 - Mimo mapy:**
```cpp
env.isOccupied(30.0, 20.0)  // Ak mapa je 20x20 → TRUE (mimo mapy)
```

**Prípad 3 - Vo voľnom priestore:**
```cpp
env.isOccupied(5.0, 5.0)    // Ak pixel je biely (255) → FALSE
```

**Prípad 4 - Na prekážke:**
```cpp
env.isOccupied(2.0, 2.0)    // Ak pixel je čierny (0) → TRUE
```

---

#### getWidth() a getHeight()
```cpp
double getWidth() const;
double getHeight() const;
```
**Čo to robí:** Vracia rozmery mapy v metroch  
**Výpočet:**
```
width_meters = png_width_pixels * resolution
// Príklad: 600 pixelov * 0.05 = 30 metrov
```

---

## 4️⃣ Environment.cpp - Implementácia Mapy

### Konštruktor
```cpp
Environment::Environment(const Config& config)
    : resolution_(config.resolution) {
    if (resolution_ <= 0.0) {
        throw std::invalid_argument("Environment resolution must be positive.");
    }

    map_ = cv::imread(config.map_filename, cv::IMREAD_GRAYSCALE);
    if (map_.empty()) {
        throw std::runtime_error("Failed to load map file: " + config.map_filename);
    }
}
```
**Čo to robí:**
1. Skopíruje `resolution` do členskej premennej
2. **Kontrola:** Ak `resolution ≤ 0` → vyhadzuje `std::invalid_argument`
3. **Načítanie:** `cv::imread()` načíta PNG v odtieňoch sivej (GRAYSCALE)
   - Biely pixel (255) = voľný priestor
   - Čierny pixel (0) = prekážka
4. **Kontrola:** Ak je `map_.empty()` (súbor neexistuje) → vyhadzuje `std::runtime_error`

**Príklad chyby:**
```cpp
// Súbor neexistuje
environment::Config bad_config;
bad_config.map_filename = "nonexistent.png";  // 🚫 Neexistuje!
bad_config.resolution = 0.05;

try {
    environment::Environment env(bad_config);
} catch (const std::runtime_error& e) {
    std::cerr << e.what();  // "Failed to load map file: nonexistent.png"
}
```

---

### isOccupied() - Detekcia Prekážky
```cpp
bool Environment::isOccupied(double x, double y) const {
    // Krok 1: Kontrola negatívnych súradníc
    if (x < 0.0 || y < 0.0) {
        return true;  // Negatívne = mimo mapy = prekážka
    }

    // Krok 2: Konverzia metrov na pixely
    const int px = static_cast<int>(std::floor(x / resolution_));
    const int py = static_cast<int>(std::floor(y / resolution_));

    // Krok 3: Kontrola hraníc mapy
    if (px < 0 || py < 0 || px >= map_.cols || py >= map_.rows) {
        return true;  // Mimo mapy = prekážka
    }

    // Krok 4: Prečítanie pixelu a kontrola barvy
    const unsigned char value = map_.at<unsigned char>(py, px);
    return value == 0;  // TRUE ak je čierny (0) = prekážka
}
```

**Detailný Príklad:**
```
Scenario: Robot na pozícii (0.5m, 0.5m), resolution = 0.05

Krok 1: x=0.5 ≥ 0 ✓, y=0.5 ≥ 0 ✓  → Pokračuj

Krok 2: Konverzia pixelov
  px = floor(0.5 / 0.05) = floor(10) = 10
  py = floor(0.5 / 0.05) = floor(10) = 10

Krok 3: Hranice
  png.cols = 600, png.rows = 400
  px=10 je v [0, 600) ✓
  py=10 je v [0, 400) ✓

Krok 4: Prečítanie
  Otvorím PNG na pixeli [10][10]
  Ak je čierny (0) → vráť TRUE (prekážka)
  Ak je biely (255) → vráť FALSE (voľné)
```

---

### getWidth() a getHeight()
```cpp
double Environment::getWidth() const {
    return static_cast<double>(map_.cols) * resolution_;
}

double Environment::getHeight() const {
    return static_cast<double>(map_.rows) * resolution_;
}
```
**Príklad:**
```
PNG: 600x400 pixelov, resolution: 0.05

getWidth()  = 600 * 0.05 = 30.0 metrov
getHeight() = 400 * 0.05 = 20.0 metrov
```

---

## 5️⃣ Lidar.h - Deklarácia LIDAR Senzoru

### Config Štruktúra
```cpp
namespace lidar {

struct Config {
    double max_range;           // Maximálny dosah (10 metrov)
    int    beam_count;          // Počet lúčov (60)
    double first_ray_angle;     // Prvý lúč (-π radiánu)
    double last_ray_angle;      // Posledný lúč (+π radiánu)
};
```
**Príklad:**
```cpp
lidar::Config config;
config.max_range = 10.0;          // Vidí do 10m
config.beam_count = 60;           // 60 lúčov
config.first_ray_angle = -M_PI;   // -180°
config.last_ray_angle = M_PI;     // +180°
```

---

### Lidar Trieda
```cpp
class Lidar {
public:
    Lidar(const Config& config, std::shared_ptr<environment::Environment> env);
    
    std::vector<geometry::Point2d> scan(const geometry::RobotState& state) const;

private:
    Config config_;
    std::shared_ptr<environment::Environment> env_;
};
```

**Metódy:**

#### Konštruktor
```cpp
Lidar(const Config& config, std::shared_ptr<environment::Environment> env);
```
- Uloží konfiguráciu
- Uloží odkaz na Environment (mapu)

---

#### scan()
```cpp
std::vector<geometry::Point2d> scan(const geometry::RobotState& state) const;
```
**Čo to robí:** Simuluje LIDAR senzor  
**Vstup:** RobotState - pozícia a orientácia robota  
**Výstup:** Vektor 60 bodov (bodov dotyku lúčov)  
**Ako sa používa:**
```cpp
geometry::RobotState state;
state.x = 5.0;
state.y = 3.0;
state.theta = 0.0;

auto hits = lidar.scan(state);
// hits je vektor s 60 bodmi kde lúče narazia na steny
```

---

## 6️⃣ Lidar.cpp - Implementácia LIDAR Skenu

### Konštruktor
```cpp
Lidar::Lidar(const Config& config, std::shared_ptr<environment::Environment> env)
    : config_(config), env_(std::move(env)) {
}
```
**Čo to robí:** Inicializuje konfiguráciu a Environment odkaz

---

### scan() - Ray Casting Algoritmus
```cpp
std::vector<geometry::Point2d> Lidar::scan(const geometry::RobotState& state) const {
    std::vector<geometry::Point2d> hits;
    
    // Kontrola platnosti
    if (!env_ || config_.beam_count <= 0 || config_.max_range <= 0.0) {
        return hits;  // Vráť prázdny vektor ak je chyba
    }

    hits.reserve(static_cast<size_t>(config_.beam_count));
```
**Čo to robí:**
- Vytvorí prázdny vektor `hits` na uloženie bodov
- Kontroluje či je Environment platný
- `reserve()` prealokuje pamäť pre 60 bodov (efektivita)

---

### Vypočítanie Uhla Medzi Lúčmi
```cpp
    const double step = 0.02;  // Každý lúč skúša každých 2cm
    const int beam_count = config_.beam_count;
    const double ray_span = config_.last_ray_angle - config_.first_ray_angle;
    // ray_span = π - (-π) = 2π (360°)
    
    const double beam_step = (beam_count > 1) 
        ? (ray_span / static_cast<double>(beam_count - 1)) 
        : 0.0;
    // beam_step = 2π / 59 ≈ 0.1065 radiánu
```
**Čo to robí:**
- `step = 0.02` - lúč ide po 2cm krokoch (presnosť)
- `ray_span = 2π` - celý kruh (360°)
- `beam_step` - rozdelí 360° na 60 lúčov

**Vizuálne:**
```
     90°
      |
180°--+--0°
      |
     270°

Lúče:
-180°, -173.9°, -167.8°, ... 0°, ... +167.8°, +173.9°, +180°
(Spolu 60 lúčov rovnomerne rozdelených)
```

---

### Hlavný Cyklus - Ray Casting
```cpp
    for (int i = 0; i < beam_count; ++i) {
        // Vypočítaj uhol i-teho lúča
        const double rel_angle = config_.first_ray_angle + beam_step * i;
        const double angle = state.theta + rel_angle;
```
**Čo to robí:**
- Pre každý lúč (0-59):
- `rel_angle` = uhol lúča relatívne k robotovi (-π, -π+0.1065, ...)
- `angle` = aktuálny uhol v mappe (priadá sa orientácia robota)

**Príklad:**
```
Robot je natočený na 45° (state.theta = 0.785 rad)
1. lúč: rel_angle = -π, angle = -π + 0.785 (lúč ide na západ-juh)
2. lúč: rel_angle = -π + 0.1065, angle = -π + 0.1065 + 0.785
```

---

### Výpočet Koncového Bodu Lúča
```cpp
        geometry::Point2d endpoint{
            state.x + config_.max_range * std::cos(angle),
            state.y + config_.max_range * std::sin(angle)
        };
```
**Čo to robí:** Vypočíta koncový bod keby lúč nenarazil na nič  
**Vzorec:**
```
x_end = x_robot + 10m * cos(angle)
y_end = y_robot + 10m * sin(angle)
```

**Príklad:**
```
Robot: (5, 5), max_range: 10m, angle: 0° (smerom vpravo)

endpoint.x = 5 + 10 * cos(0) = 5 + 10 * 1 = 15
endpoint.y = 5 + 10 * sin(0) = 5 + 10 * 0 = 5

endpoint = (15, 5)  ← 10m vpravo od robota
```

---

### Ray Casting Cyklus - Hľadanie Prekážky
```cpp
        for (double range = step; range <= config_.max_range; range += step) {
            // Vypočítaj pozíciu na lúči v danej vzdialenosti
            const double x = state.x + range * std::cos(angle);
            const double y = state.y + range * std::sin(angle);

            // Skúšaj či je táto pozícia prekážka
            if (env_->isOccupied(x, y)) {
                endpoint = {x, y};  // Našli sme prekážku!
                break;  // Prestaj hľadať
            }
        }
```
**Čo to robí:** Ray Casting algoritmus:
1. Začína na `range = 0.02` (2cm od robota)
2. Posúva sa po 2cm krokoch: 0.02, 0.04, 0.06, ..., 10.0
3. Na každom kroku:
   - Vypočíta pozíciu: `x = x_robot + range * cos(angle)`
   - Skúša `isOccupied(x, y)` = je tam prekážka?
4. Keď nájde prekážku → uloží bod a prestane

**Vizuálny Príklad:**
```
Robot na (0, 0), lúč smerom vpravo (0°), prekážka na 3.5m

range=0.02:  (0.02, 0) → voľné ✓
range=0.04:  (0.04, 0) → voľné ✓
range=0.06:  (0.06, 0) → voľné ✓
...
range=3.48:  (3.48, 0) → voľné ✓
range=3.50:  (3.50, 0) → PREKÁŽKA! 🚫 STOP!
endpoint = (3.50, 0)
```

---

### Uloženie Bodu
```cpp
        hits.push_back(endpoint);
    }

    return hits;
}
```
**Čo to robí:** Uloží bod do vektora a pokračuje ďalším lúčom  
**Výsledok:** Vektor s 60 bodmi dotyku lúčov

---

## 7️⃣ Canvas.h - Deklarácia Vizualizácie

### Config Štruktúra
```cpp
namespace canvas {

struct Config {
    std::string map_filename;
    double resolution;
};
```

---

### Canvas Trieda
```cpp
class Canvas {
public:
    explicit Canvas(const Config& config);
    
    bool loadMap();
    void resetBackground();
    void drawPointPixel(int x, int y, const cv::Scalar& color, int radius);
    void drawLine(int x1, int y1, int x2, int y2, const cv::Scalar& color, int thickness);
    void drawRobot(int x, int y, const cv::Scalar& color, int radius);
    void display(const std::string& window_name) const;

    const cv::Mat& getBackground() const;
    const cv::Mat& getDisplay() const;

private:
    Config config_;
    cv::Mat background_;  // Pôvodná mapa
    cv::Mat display_;     // Mapa na kreslenie
};
```

**Premenné:**
- `background_` = pôvodná PNG mapa (nikdy sa nemení)
- `display_` = kópia mapy na kreslenie robotov a lúčov

---

## 8️⃣ Canvas.cpp - Implementácia Vizualizácie

### loadMap() - Načítanie PNG
```cpp
bool Canvas::loadMap() {
    background_ = cv::imread(config_.map_filename, cv::IMREAD_COLOR);
    if (background_.empty()) {
        return false;
    }
    background_.copyTo(display_);
    return true;
}
```
**Čo to robí:**
1. Načíta PNG do `background_` (farebný obrázok - BGR format)
2. Skopíruje ho do `display_`
3. Vracia `FALSE` ak súbor neexistuje

---

### resetBackground() - Vyčistenie Kreslenia
```cpp
void Canvas::resetBackground() {
    if (!background_.empty()) {
        background_.copyTo(display_);
    }
}
```
**Čo to robí:**
- Skopíruje pôvodnú mapu do `display_`
- Vymaže všetko čo bolo nakreslené v predchádzajúcom frame-e
- Príprava na nové kreslenie

---

### drawPointPixel() - Nakreslenie Bodky
```cpp
void Canvas::drawPointPixel(int x, int y, const cv::Scalar& color, int radius) {
    if (display_.empty()) return;
    if (x < 0 || y < 0 || x >= display_.cols || y >= display_.rows) return;
    
    cv::circle(display_, cv::Point(x, y), radius, color, -1);
}
```
**Čo to robí:**
1. Kontroluje hranice (bod je v mape?)
2. Nakreslí plný kruh (`-1` = vyplnený)
3. `color` = farba v BGR formáte
4. `radius` = polomer v pixeloch

**Príklad:**
```cpp
// Nakreslí zelený kruh s polomerom 5px na pozícii (100, 100)
canvas.drawPointPixel(100, 100, cv::Scalar(0, 255, 0), 5);
// cv::Scalar(B, G, R) = BGR format!
```

---

### drawLine() - Nakreslenie Čiary
```cpp
void Canvas::drawLine(int x1, int y1, int x2, int y2, 
                      const cv::Scalar& color, int thickness) {
    if (display_.empty()) return;
    
    cv::line(display_, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness);
}
```
**Čo to robí:** Nakreslí čiaru z bodu (x1, y1) do (x2, y2)  
**Príklad:**
```cpp
// Nakreslí sivú čiaru z (50, 50) do (150, 100)
canvas.drawLine(50, 50, 150, 100, cv::Scalar(128, 128, 128), 1);
```

---

### drawRobot() - Nakreslenie Robota
```cpp
void Canvas::drawRobot(int x, int y, const cv::Scalar& color, int radius) {
    if (display_.empty()) return;
    
    cv::circle(display_, cv::Point(x, y), radius, color, -1);
}
```
**Čo to robí:** Rovnako ako `drawPointPixel()` ale špecificky pre robota  
**Príklad:**
```cpp
// Nakreslí zelený kruh (robota) s polomerom 5px
canvas.drawRobot(200, 150, cv::Scalar(0, 255, 0), 5);
```

---

### display() - Zobrazenie Mapy
```cpp
void Canvas::display(const std::string& window_name) const {
    if (!display_.empty()) {
        cv::imshow(window_name, display_);
    }
}
```
**Čo to robí:** Zobrazí `display_` mapu v OpenCV okne  
**Príklad:**
```cpp
canvas.display("Map");  // Zobrazí v okne s názvom "Map"
```

---

## 9️⃣ main.cpp - Demo Aplikácia

### Mouse Callback - Zachytenie Kliknutí
```cpp
struct MouseData {
    std::vector<cv::Point>* points;
};

void mouseCallback(int event, int x, int y, int, void* userdata) {
    if (event != cv::EVENT_LBUTTONDOWN || userdata == nullptr) {
        return;
    }
    auto* data = static_cast<MouseData*>(userdata);
    data->points->emplace_back(x, y);
}
```
**Čo to robí:**
- `MouseData` = struktura na prenos dát do callback-u
- `mouseCallback()` = sa zavolá keď sa stane niečo s myšou
- Skúša či to bol **ľavý klik** (`EVENT_LBUTTONDOWN`)
- Pridá kliknutý bod do vektora

---

### Inicializácia Parametrov
```cpp
int main() {
    canvas::Config config;
    config.map_filename = "../opk-map.png";
    config.resolution = 0.05;
```
**Čo to robí:** Nastaví cestu k mape a rozlíšenie (5cm/pixel)

---

### Vytvorenie Environment-u
```cpp
    environment::Config env_config;
    env_config.map_filename = config.map_filename;
    env_config.resolution = config.resolution;
    auto environment = std::make_shared<environment::Environment>(env_config);
```
**Čo to robí:**
- Vytvorí Environment
- Načíta PNG mapu do pamäte
- Vyhadzuje exception ak chyba

---

### Vytvorenie Lidar-u
```cpp
    constexpr double kPi = 3.14159265358979323846;
    lidar::Config lidar_config;
    lidar_config.max_range = 10.0;
    lidar_config.beam_count = 60;
    lidar_config.first_ray_angle = -kPi;
    lidar_config.last_ray_angle = kPi;
    lidar::Lidar lidar(lidar_config, environment);
```
**Čo to robí:**
- Vytvorí Lidar s konfiguráciou
- 60 lúčov
- Dosah 10 metrov
- Pokrýva -180° až +180°

---

### Vytvorenie Canvas-u a Načítanie Mapy
```cpp
    canvas::Canvas map_canvas(config);
    if (!map_canvas.loadMap()) {
        std::cerr << "Nepodarilo sa nacitat mapu: " << config.map_filename << '\n';
        return 1;
    }
```
**Čo to robí:**
- Vytvorí Canvas
- Načíta PNG mapu
- Ak chyba, vypíše chybovú správu a skončí

---

### OpenCV Okno a Mouse Callback
```cpp
    cv::namedWindow("Map", cv::WINDOW_AUTOSIZE);
    std::vector<cv::Point> clicked_points;
    MouseData mouse_data{&clicked_points};
    cv::setMouseCallback("Map", mouseCallback, &mouse_data);
```
**Čo to robí:**
1. Vytvorí OpenCV okno s názvom "Map"
2. Vytvorí vektor na uloženie kliknutých bodov
3. Zaregistruje mouse callback - keď klikneš, zavola sa `mouseCallback()`

---

### Hlavný Loop - Vykresľovanie
```cpp
    bool running = true;
    while (running) {
        map_canvas.resetBackground();
```
**Čo to robí:** Každý frame: vymaže kreslenie z minulého frame-u

---

### Vykresľovanie Robotov
```cpp
        for (const auto& p : clicked_points) {
            // Vykresliť robota - zelený kruh
            map_canvas.drawRobot(p.x, p.y, cv::Scalar(0, 255, 0), 5);
```
**Čo to robí:** Pre každý kliknutý bod nakreslí zelený kruh (robot)

---

### Vytvorenie RobotState
```cpp
            geometry::RobotState state{};
            state.x = static_cast<double>(p.x) * config.resolution;
            state.y = static_cast<double>(p.y) * config.resolution;
            state.theta = 0.0;
            state.velocity = {0.0, 0.0};
```
**Čo to robí:**
1. Vytvorí RobotState pre LIDAR scan
2. Konvertuje pixely na metre: `pixel * 0.05`
3. Robot je natočený na 0° (smerom vpravo)
4. Robot je nehybný

**Príklad:**
```
Klikol si na pixel (100, 200)

state.x = 100 * 0.05 = 5.0 metrov
state.y = 200 * 0.05 = 10.0 metrov
```

---

### LIDAR Scan
```cpp
            const auto hits = lidar.scan(state);
```
**Čo to robí:** Spustí LIDAR - vracia vektor 60 bodov dotyku lúčov

---

### Vykresľovanie Lidar Lúčov
```cpp
            for (const auto& hit : hits) {
                const int hx = static_cast<int>(std::lround(hit.x / config.resolution));
                const int hy = static_cast<int>(std::lround(hit.y / config.resolution));
                
                // Nakreslí čiaru (lidar lúč)
                map_canvas.drawLine(p.x, p.y, hx, hy, cv::Scalar(128, 128, 128), 1);
                
                // Nakreslí bodku na konci lúča
                map_canvas.drawPointPixel(hx, hy, cv::Scalar(128, 128, 128), 1);
            }
```
**Čo to robí:**
1. Pre každý bod dotyku lúča:
2. Konvertuje spätne z metrov na pixely: `meter / 0.05`
3. Nakreslí sivú čiaru z robota k bodu (LIDAR lúč)
4. Nakreslí sivú bodku na konci lúča

**Príklad:**
```
Robot na pixeli (100, 200)
LIDAR hit na metre (7.5, 10.0)

hx = 7.5 / 0.05 = 150
hy = 10.0 / 0.05 = 200

Nákresli čiaru:   (100, 200) → (150, 200)  [sivá čiara]
Nákresli bodku:   (150, 200)               [sivá bodka]
```

---

### Vykresľovanie Zelených Bodiek NAD Lúčami
```cpp
        for (const auto& p : clicked_points) {
            map_canvas.drawPointPixel(p.x, p.y, cv::Scalar(0, 255, 0), 2);
        }
```
**Čo to robí:**
- Nakreslí zelené bodky ROBOTOV NAD sivými lúčami
- **Dôležité:** Poradie kreslenia! Najprv lúče, potom roboty
- Takže roboty sú vždy viditeľné

---

### Zobrazenie a ESC
```cpp
        map_canvas.display("Map");
        const int key = cv::waitKey(30);
        if (key == 27) {
            running = false;
        }
    }

    cv::destroyAllWindows();
    return 0;
}
```
**Čo to robí:**
1. Zobrazí mapu v okne
2. `cv::waitKey(30)` - čaká 30ms (aby bolo vidieť animation)
3. Ak kľúč == 27 (ESC) → ukončí program
4. Na konci zatvoriť všetky okná

---

## 🟟 robot_test.cpp - Unit Testy

### Environment Testy (5 testov)

#### Test 1: LoadNonExistentFile
```cpp
TEST(EnvironmentTest, LoadNonExistentFile) {
    environment::Config config;
    config.map_filename = "nonexistent_map.png";
    config.resolution = 0.05;

    EXPECT_THROW({
        environment::Environment env(config);
    }, std::runtime_error);
}
```
**Čo to testuje:** Ak PNG súbor neexistuje, konštruktor vyhadzuje exception  
**Očakávaný výsledok:** ✅ PASS (exception je vyhozený)

---

#### Test 2: InvalidResolution
```cpp
TEST(EnvironmentTest, InvalidResolution) {
    environment::Config config;
    config.map_filename = "../opk-map.png";
    config.resolution = -0.05;

    EXPECT_THROW({
        environment::Environment env(config);
    }, std::invalid_argument);
}
```
**Čo to testuje:** Ak je resolution záporné (-0.05), vyhadzuje exception  
**Očakávaný výsledok:** ✅ PASS (exception je vyhozený)

---

#### Test 3: OccupiedOutsideMap
```cpp
TEST(EnvironmentTest, OccupiedOutsideMap) {
    environment::Config config;
    config.map_filename = "../opk-map.png";
    config.resolution = 0.05;
    environment::Environment env(config);

    // Negatívne súradnice
    EXPECT_TRUE(env.isOccupied(-1.0, -1.0));

    // Mimo mapy
    double width = env.getWidth();
    double height = env.getHeight();
    EXPECT_TRUE(env.isOccupied(width + 1.0, height + 1.0));
}
```
**Čo to testuje:**
1. Negatívne súradnice (-1, -1) sú považované za prekážku
2. Súradnice mimo mapy (width+1, height+1) sú prekážka

**Očakávaný výsledok:** ✅ PASS (oba sú TRUE)

---

#### Test 4: MapDimensions
```cpp
TEST(EnvironmentTest, MapDimensions) {
    environment::Config config;
    config.map_filename = "../opk-map.png";
    config.resolution = 0.05;
    environment::Environment env(config);

    double width = env.getWidth();
    double height = env.getHeight();

    EXPECT_GT(width, 0.0);
    EXPECT_GT(height, 0.0);
}
```
**Čo to testuje:** Rozmery mapy sú kladné (width > 0 a height > 0)  
**Očakávaný výsledok:** ✅ PASS (obe sú > 0)

---

#### Test 5: FreeSpace
```cpp
TEST(EnvironmentTest, FreeSpace) {
    environment::Config config;
    config.map_filename = "../opk-map.png";
    config.resolution = 0.05;
    environment::Environment env(config);

    bool found_free_space = false;
    for (double x = 0.0; x < env.getWidth(); x += 0.5) {
        for (double y = 0.0; y < env.getHeight(); y += 0.5) {
            if (!env.isOccupied(x, y)) {
                found_free_space = true;
                break;
            }
        }
        if (found_free_space) break;
    }

    EXPECT_TRUE(found_free_space);
}
```
**Čo to testuje:** V mape existuje aspoň jeden voľný priestor (biely pixel)  
**Ako funguje:**
1. Prejde všetky súradnice s krokom 0.5m
2. Skúša `isOccupied()` na každej pozícii
3. Keď nájde voľný priestor → break

**Očakávaný výsledok:** ✅ PASS (mapa má voľný priestor)

---

### Lidar Testy (6 testov)

#### Test 6: LidarInitialization
```cpp
TEST(LidarTest, LidarInitialization) {
    environment::Config env_config;
    env_config.map_filename = "../opk-map.png";
    env_config.resolution = 0.05;
    auto env = std::make_shared<environment::Environment>(env_config);

    lidar::Config config;
    config.max_range = 10.0;
    config.beam_count = 60;
    config.first_ray_angle = -3.14159;
    config.last_ray_angle = 3.14159;

    lidar::Lidar lidar(config, env);
    EXPECT_TRUE(true);
}
```
**Čo to testuje:** LIDAR sa inicializuje bez chyby  
**Očakávaný výsledok:** ✅ PASS (bez exception-u)

---

#### Test 7: ScanFromCenter
```cpp
TEST(LidarTest, ScanFromCenter) {
    // ... vytvorenie env a lidar ...
    
    geometry::RobotState state;
    state.x = 5.0;
    state.y = 5.0;
    state.theta = 0.0;
    state.velocity = {0.0, 0.0};

    auto hits = lidar.scan(state);
    
    EXPECT_EQ(hits.size(), static_cast<size_t>(config.beam_count));
}
```
**Čo to testuje:** `scan()` vracia presne 60 bodov  
**Očakávaný výsledok:** ✅ PASS (veľkosť vektora == 60)

---

#### Test 8: BeamCount
```cpp
TEST(LidarTest, BeamCount) {
    // ... 
    lidar::Config config;
    config.beam_count = 30;  // ← Iný počet!
    // ...
    
    auto hits = lidar.scan(state);
    EXPECT_EQ(hits.size(), static_cast<size_t>(config.beam_count));
}
```
**Čo to testuje:** S 30 lúčmi vráti 30 bodov  
**Očakávaný výsledok:** ✅ PASS (veľkosť == 30)

---

#### Test 9: HitsWithinMaxRange
```cpp
TEST(LidarTest, HitsWithinMaxRange) {
    // ...
    auto hits = lidar.scan(state);

    for (const auto& hit : hits) {
        double distance = std::sqrt(
            (hit.x - state.x) * (hit.x - state.x) +
            (hit.y - state.y) * (hit.y - state.y)
        );
        EXPECT_LE(distance, config.max_range + 0.1);
    }
}
```
**Čo to testuje:** Všetky body sú do max_range (≤ 10.1m)  
**Ako funguje:**
1. Pre každý bod dotyku
2. Vypočíta vzdialenosť Euklidovskou formulu
3. Skúša `distance ≤ 10.1`

**Očakávaný výsledok:** ✅ PASS (všetky body sú do rozsahu)

---

#### Test 10: ScanWithNullEnvironment
```cpp
TEST(LidarTest, ScanWithNullEnvironment) {
    lidar::Config config;
    config.max_range = 10.0;
    config.beam_count = 60;
    // ...
    
    lidar::Lidar lidar(config, nullptr);  // ← NULL environment!
    
    auto hits = lidar.scan(state);
    
    EXPECT_EQ(hits.size(), 0);
}
```
**Čo to testuje:** Keď je Environment NULL, vracia prázdny vektor  
**Očakávaný výsledok:** ✅ PASS (veľkosť == 0)

---

#### Test 11: ScanWithDifferentAngles
```cpp
TEST(LidarTest, ScanWithDifferentAngles) {
    // ...
    for (double theta = 0.0; theta < 6.28; theta += 1.57) {
        state.theta = theta;  // 0°, 90°, 180°, 270°
        auto hits = lidar.scan(state);
        EXPECT_EQ(hits.size(), static_cast<size_t>(config.beam_count));
    }
}
```
**Čo to testuje:** Bez ohľadu na orientáciu robota vždy vracia 60 bodov  
**Ako funguje:**
1. Skúša 4 uhly: 0°, 90°, 180°, 270°
2. Pre každý uhol spustí scan
3. Skúša či vracia 60 bodov

**Očakávaný výsledok:** ✅ PASS (všetky vrátia 60 bodov)

---

## 🚀 Ako Spustiť Projekt

### Build
```bash
cd /home/tomas/CLionProjects/untitled/OPK/cv4_part1/build
cmake ..
make
```

### Spustenie Demo Aplikácie
```bash
./cv4_part1
```
**Ako Používať:**
1. Otvorí sa okno s mapou
2. **Ľavé kliknutie myšou** = pridá robota na tej pozícii
3. Uvidíš:
   - 🟢 Zelený kruh = robot
   - 🟣 Sivé čiary = lidar lúče
   - 🟤 Sivé bodky = body dotyku
4. **ESC** = ukončí program

### Spustenie Testov
```bash
./cv4_tests
```
Alebo:
```bash
ctest
```

**Výstup:**
```
[==========] Running 11 tests from 2 test suites.
[       OK ] EnvironmentTest.LoadNonExistentFile (X ms)
[       OK ] EnvironmentTest.InvalidResolution (X ms)
[       OK ] EnvironmentTest.OccupiedOutsideMap (X ms)
[       OK ] EnvironmentTest.MapDimensions (X ms)
[       OK ] EnvironmentTest.FreeSpace (X ms)
[       OK ] LidarTest.LidarInitialization (X ms)
[       OK ] LidarTest.ScanFromCenter (X ms)
[       OK ] LidarTest.BeamCount (X ms)
[       OK ] LidarTest.HitsWithinMaxRange (X ms)
[       OK ] LidarTest.ScanWithNullEnvironment (X ms)
[       OK ] LidarTest.ScanWithDifferentAngles (X ms)
[==========] 11 passed in X ms
```

---

## 📊 Architektura Projektu - Diagram Toku

```
main.cpp
    │
    ├─→ Environment ("opk-map.png")
    │       │
    │       ├─ Načítá PNG (cv::imread)
    │       ├─ Triedi pixely: 255=voľné, 0=prekážka
    │       └─ Kontrola: isOccupied(x, y)
    │
    ├─→ Lidar (60 lúčov, 10m dosah)
    │       │
    │       ├─ scan(RobotState) → Vracia 60 bodov
    │       │    └─ Ray Casting algoritmus
    │       │         └─ Skúša každé 2cm až do 10m
    │       └─ Vracia std::vector<Point2d>
    │
    ├─→ Canvas (Vizualizácia)
    │       │
    │       ├─ loadMap() - načíta PNG
    │       ├─ drawLine() - nakreslí čiaru (lúč)
    │       ├─ drawPointPixel() - nakreslí bodku
    │       ├─ drawRobot() - nakreslí robota
    │       └─ display() - zobrazí v okne
    │
    └─→ Mouse Callback
            │
            └─ Zaznamená kliknutia
                └─ Vytvorí RobotState
                    └─ Spustí Lidar.scan()
```

---

## 🎯 Čo Projekt Spĺňa (Zadanie Časť 1)

✅ **CMakeLists.txt** - Build systém s OpenCV a Google Test  
✅ **Environment** - Načítanie PNG a detekcia prekážok  
✅ **Lidar** - Ray Casting algoritmus (60 lúčov)  
✅ **Canvas** - Vizualizácia pomocou OpenCV  
✅ **Demo Aplikácia** - Interaktívne klikanie a vykresľovanie  
✅ **Unit Testy** - 11 testov (5 Environment + 6 Lidar)  

---

## 💾 Kľúčové Koncepty

- **Ray Casting** - algoritmus na hľadanie prvej prekážky v ceste lúča
- **Konverzia Súradníc** - pixely ↔ metre pomocou `resolution`
- **2D Geometria** - uhly (radiány), trigonometria (sin, cos)
- **OpenCV** - načítanie obrázkov, kreslenie a zobrazovanie
- **Unit Testing** - Google Test Framework na overovanie funkcionalnosti
- **Error Handling** - Výnimky na ošetrenie chýb
- **Shared Pointers** - `std::shared_ptr` na správu pamäte

---

**Hotovo! 📚**

