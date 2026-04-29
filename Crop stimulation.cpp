#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

/*// Weather conditions enumeration
enum Weather { SUNNY, CLOUDY, RAINY, STORM };

// Crop class to manage individual crop properties
class Crop {
private:
    string cropName;
    double growthStage;      // 0.0 to 1.0 (0% to 100% growth)
    double health;           // 0.0 to 1.0
    double waterLevel;       // 0.0 to 1.0
    double nutrientLevel;    // 0.0 to 1.0
    int daysOld;
    bool isHarvestable;
    
public:
    Crop(string name) {
        cropName = name;
        growthStage = 0.0;
        health = 1.0;
        waterLevel = 0.5;
        nutrientLevel = 0.5;
        daysOld = 0;
        isHarvestable = false;
    }
    
    void displayStatus() {
        cout << "\n=== " << cropName << " Status ===" << endl;
        cout << "Days Old: " << daysOld << endl;
        cout << "Growth Stage: " << fixed << setprecision(1) << (growthStage * 100) << "%" << endl;
        cout << "Health: " << (health * 100) << "%" << endl;
        cout << "Water Level: " << (waterLevel * 100) << "%" << endl;
        cout << "Nutrient Level: " << (nutrientLevel * 100) << "%" << endl;
        cout << "Harvestable: " << (isHarvestable ? "Yes" : "No") << endl;
        
        // Draw growth bar
        cout << "Growth: [";
        int bars = growthStage * 20;
        for (int i = 0; i < 20; i++) {
            if (i < bars) cout << "=";
            else if (i == bars) cout << ">";
            else cout << " ";
        }
        cout << "]" << endl;
    }
    
    void waterCrop(double amount) {
        waterLevel += amount;
        if (waterLevel > 1.0) waterLevel = 1.0;
        if (waterLevel < 0.0) waterLevel = 0.0;
        cout << "Watered " << cropName << ". New water level: " << (waterLevel * 100) << "%" << endl;
    }
    
    void fertilize(double amount) {
        nutrientLevel += amount;
        if (nutrientLevel > 1.0) nutrientLevel = 1.0;
        if (nutrientLevel < 0.0) nutrientLevel = 0.0;
        cout << "Fertilized " << cropName << ". New nutrient level: " << (nutrientLevel * 100) << "%" << endl;
    }
    
    void updateGrowth(Weather weather, int temperature) {
        daysOld++;
        
        // Calculate growth rate based on conditions
        double growthRate = 0.05; // Base growth rate per day
        
        // Weather effects
        switch(weather) {
            case SUNNY:
                growthRate *= 1.2;
                waterLevel -= 0.05; // Evaporation
                break;
            case CLOUDY:
                growthRate *= 0.9;
                waterLevel -= 0.02;
                break;
            case RAINY:
                growthRate *= 1.1;
                waterLevel += 0.1;
                break;
            case STORM:
                growthRate *= 0.7;
                waterLevel += 0.15;
                health -= 0.05; // Storm damage
                break;
        }
        
        // Temperature effects
        if (temperature < 10) growthRate *= 0.5;  // Too cold
        else if (temperature < 15) growthRate *= 0.8;
        else if (temperature < 30) growthRate *= 1.2; // Optimal
        else if (temperature < 35) growthRate *= 0.9;
        else growthRate *= 0.6; // Too hot
        
        // Water and nutrient effects
        growthRate *= (0.5 + waterLevel);
        growthRate *= (0.5 + nutrientLevel);
        
        // Health effect
        growthRate *= health;
        
        // Apply growth
        growthStage += growthRate / 100.0;
        if (growthStage >= 1.0) {
            growthStage = 1.0;
            isHarvestable = true;
        }
        
        // Health degradation if conditions are poor
        if (waterLevel < 0.2) health -= 0.03;
        if (nutrientLevel < 0.2) health -= 0.02;
        if (temperature > 35 || temperature < 5) health -= 0.05;
        
        // Ensure health stays within bounds
        if (health < 0) health = 0;
        if (health > 1) health = 1;
        
        // Natural water and nutrient loss
        waterLevel -= 0.02;
        nutrientLevel -= 0.01;
        
        // Clamp values
        if (waterLevel < 0) waterLevel = 0;
        if (nutrientLevel < 0) nutrientLevel = 0;
    }
    
    double harvest() {
        if (!isHarvestable) {
            cout << cropName << " is not ready for harvest yet!" << endl;
            return 0;
        }
        
        double yield = growthStage * health * waterLevel * nutrientLevel * 100;
        cout << "Harvested " << cropName << "! Yield: " << fixed << setprecision(1) << yield << " units" << endl;
        isHarvestable = false;
        return yield;
    }
    
    bool isReady() { return isHarvestable; }
    string getName() { return cropName; }
    double getHealth() { return health; }
};

// Weather generator class
class WeatherSystem {
private:
    Weather currentWeather;
    int temperature;
    
public:
    WeatherSystem() {
        srand(time(0));
        generateRandomWeather();
    }
    
    void generateRandomWeather() {
        int weatherCode = rand() % 100;
        if (weatherCode < 50) currentWeather = SUNNY;
        else if (weatherCode < 75) currentWeather = CLOUDY;
        else if (weatherCode < 92) currentWeather = RAINY;
        else currentWeather = STORM;
        
        // Generate temperature (5°C to 40°C)
        temperature = 5 + (rand() % 36);
    }
    
    string getWeatherString() {
        switch(currentWeather) {
            case SUNNY: return "?? Sunny";
            case CLOUDY: return "?? Cloudy";
            case RAINY: return "??? Rainy";
            case STORM: return "?? Storm";
            default: return "Unknown";
        }
    }
    
    Weather getWeather() { return currentWeather; }
    int getTemperature() { return temperature; }
    
    void displayForecast() {
        cout << "\n?? Weather Forecast:" << endl;
        cout << "   Weather: " << getWeatherString() << endl;
        cout << "   Temperature: " << temperature << "°C" << endl;
        
        if (temperature < 10) cout << "   ?? Cold warning!" << endl;
        else if (temperature > 35) cout << "   ?? Heat warning!" << endl;
        else if (currentWeather == STORM) cout << "   ?? Storm warning!" << endl;
    }
};

// Farm management system
class Farm {
private:
    vector<Crop> crops;
    double totalYield;
    int dayCounter;
    WeatherSystem weather;
    
public:
    Farm() {
        totalYield = 0;
        dayCounter = 0;
    }
    
    void addCrop(string cropName) {
        crops.push_back(Crop(cropName));
        cout << "? Added " << cropName << " to your farm!" << endl;
    }
    
    void displayAllCrops() {
        if (crops.empty()) {
            cout << "? No crops on your farm yet!" << endl;
            return;
        }
        
        cout << "\n?? Your Farm Status (Day " << dayCounter << ")" << endl;
        cout << "=====================================" << endl;
        for (size_t i = 0; i < crops.size(); i++) {
            cout << i + 1 << ". ";
            crops[i].displayStatus();
        }
    }
    
    void waterAllCrops(double amount) {
        for (size_t i = 0; i < crops.size(); i++) {
            crops[i].waterCrop(amount);
        }
    }
    
    void fertilizeAllCrops(double amount) {
        for (size_t i = 0; i < crops.size(); i++) {
            crops[i].fertilize(amount);
        }
    }
    
    void nextDay() {
        dayCounter++;
        weather.generateRandomWeather();
        
        cout << "\n?? Day " << dayCounter << " - " << weather.getWeatherString() 
             << ", " << weather.getTemperature() << "°C" << endl;
        
        // Update each crop
        for (size_t i = 0; i < crops.size(); i++) {
            crops[i].updateGrowth(weather.getWeather(), weather.getTemperature());
        }
        
        // Check for crop death
        for (size_t i = 0; i < crops.size(); i++) {
            if (crops[i].getHealth() <= 0) {
                cout << "?? WARNING! " << crops[i].getName() << " has died from poor conditions!" << endl;
                crops.erase(crops.begin() + i);
                i--;
            }
        }
    }
    
    void harvestAll() {
        if (crops.empty()) {
            cout << "No crops to harvest!" << endl;
            return;
        }
        
        double dayYield = 0;
        for (size_t i = 0; i < crops.size(); i++) {
            if (crops[i].isReady()) {
                dayYield += crops[i].harvest();
            }
        }
        
        totalYield += dayYield;
        cout << "\n?? Total yield today: " << fixed << setprecision(1) << dayYield << " units" << endl;
        cout << "?? Cumulative yield: " << totalYield << " units" << endl;
    }
    
    void showFarmSummary() {
        cout << "\n?? Farm Summary Report" << endl;
        cout << "=======================" << endl;
        cout << "Days Operated: " << dayCounter << endl;
        cout << "Active Crops: " << crops.size() << endl;
        cout << "Total Harvest Yield: " << totalYield << " units" << endl;
        
        if (crops.empty()) {
            cout << "Status: Farm is empty" << endl;
        } else {
            cout << "Status: Active farm with " << crops.size() << " crops" << endl;
        }
    }
    
    void saveGame(string filename) {
        ofstream file(filename.c_str());
        if (file.is_open()) {
            file << dayCounter << endl;
            file << totalYield << endl;
            file << crops.size() << endl;
            
            for (size_t i = 0; i < crops.size(); i++) {
                // Simplified save - in real implementation would save all properties
                file << crops[i].getName() << endl;
            }
            
            file.close();
            cout << "? Game saved to " << filename << endl;
        } else {
            cout << "? Error saving game!" << endl;
        }
    }
};

// Main menu system
int main() {
    Farm myFarm;
    int choice;
    
    cout << "?????? CROP GROWTH SIMULATION SYSTEM ??????" << endl;
    cout << "============================================" << endl;
    cout << "Welcome to the Advanced Crop Growth Simulator!" << endl;
    cout << "Manage your farm, monitor weather, and grow healthy crops.\n" << endl;
    
    // Initialize with a sample crop
    myFarm.addCrop("Wheat");
    
    do {
        cout << "\n=== MAIN MENU ===" << endl;
        cout << "1. ?? View all crops" << endl;
        cout << "2. ?? Add new crop" << endl;
        cout << "3. ?? Water all crops" << endl;
        cout << "4. ?? Fertilize all crops" << endl;
        cout << "5. ?? Next day (advance simulation)" << endl;
        cout << "6. ?? Harvest ready crops" << endl;
        cout << "7. ?? View farm summary" << endl;
        cout << "8. ?? Save game" << endl;
        cout << "9. ? Exit" << endl;
        cout << "Enter your choice (1-9): ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                myFarm.displayAllCrops();
                break;
                
            case 2: {
                string cropName;
                cout << "Enter crop name: ";
                cin >> cropName;
                myFarm.addCrop(cropName);
                break;
            }
                
            case 3: {
                double waterAmount;
                cout << "Enter water amount (0.1 - 0.5): ";
                cin >> waterAmount;
                if (waterAmount < 0.1) waterAmount = 0.1;
                if (waterAmount > 0.5) waterAmount = 0.5;
                myFarm.waterAllCrops(waterAmount);
                break;
            }
                
            case 4: {
                double fertilizerAmount;
                cout << "Enter fertilizer amount (0.1 - 0.3): ";
                cin >> fertilizerAmount;
                if (fertilizerAmount < 0.1) fertilizerAmount = 0.1;
                if (fertilizerAmount > 0.3) fertilizerAmount = 0.3;
                myFarm.fertilizeAllCrops(fertilizerAmount);
                break;
            }
                
            case 5:
                myFarm.nextDay();
                break;
                
            case 6:
                myFarm.harvestAll();
                break;
                
            case 7:
                myFarm.showFarmSummary();
                break;
                
            case 8: {
                string filename;
                cout << "Enter save filename: ";
                cin >> filename;
                myFarm.saveGame(filename);
                break;
            }
                
            case 9:
                cout << "\nThank you for playing Crop Growth Simulator!" << endl;
                cout << "Final farm summary:" << endl;
                myFarm.showFarmSummary();
                break;
                
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
        
    } while (choice != 9);
return 0;*/

/*
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    // Initialization
    double soilMoisture = 50.0;
    double temperature = 25.0;
    double sunlight = 8.0;
    double cropHeight = 5.0;

    // Constants
    double r_max = 0.8;
    double M_opt = 60.0;
    double T_opt = 25.0;
    double S_opt = 10.0;

    cout << fixed << setprecision(2);

    // Simulation loop (10 days)
    for (int day = 1; day <= 10; day++) {
        double rainfall = 5.0;
        double evaporation = 2.0;
        double usage = 3.0;

        // Update soil moisture
        soilMoisture = soilMoisture + rainfall - evaporation - usage;

        // Prevent negative moisture
        if (soilMoisture < 0) {
            soilMoisture = 0;
        }

        // Calculate factors
        double f_m = soilMoisture / M_opt;
        double f_t = 1 - ((temperature - T_opt) / T_opt) * ((temperature - T_opt) / T_opt);
        double f_s = sunlight / S_opt;

        // Ensure factors are within valid range
        if (f_t < 0) f_t = 0;

        // Compute growth
        double growth = r_max * f_m * f_t * f_s;

        // Update crop height
        cropHeight += growth;

        // Output results
        cout << "Day " << day << endl;
        cout << "Soil Moisture: " << soilMoisture << "%" << endl;
        cout << "Crop Height: " << cropHeight << " cm" << endl;
        cout << "-------------------------" << endl;
    }

    return 0;
} */

#include <iostream>
#include <string>
using namespace std;

// -------------------- Crop Class --------------------
class Crop {
private:
    string name;
    float height;
    string health;

public:
    // Constructor
    Crop(string n) {
        name = n;
        height = 0.0;
        health = "Good";
    }

    // Grow function
    void grow(float growthRate) {
        height += growthRate;

        // Update health based on growth
        if (growthRate < 0.2)
            health = "Poor";
        else if (growthRate < 0.5)
            health = "Moderate";
        else
            health = "Good";
    }

    void displayStatus(int day) {
        cout << "Day " << day 
             << ": Height = " << height << " cm"
             << ", Health = " << health << endl;
    }
};

// -------------------- Environment Class --------------------
class Environment {
public:
    float rainfall;
    float temperature;
    float soilMoisture;

    void inputConditions() {
        cout << "\nEnter rainfall (mm): ";
        cin >> rainfall;

        cout << "Enter temperature (°C): ";
        cin >> temperature;

        cout << "Enter soil moisture (0-100): ";
        cin >> soilMoisture;

        // Input validation
        while (soilMoisture < 0 || soilMoisture > 100) {
            cout << "Invalid soil moisture! Enter (0-100): ";
            cin >> soilMoisture;
        }
    }

    float calculateGrowthRate() {
        float rate = 0.5; // base growth

        // Conditional logic
        if (soilMoisture < 30)
            rate -= 0.3;
        else if (soilMoisture > 70)
            rate += 0.2;

        if (rainfall > 50)
            rate += 0.2;

        if (temperature < 10 || temperature > 35)
            rate -= 0.2;

        // Ensure non-negative growth
        if (rate < 0)
            rate = 0;

        return rate;
    }
};

// -------------------- Simulation Class --------------------
class Simulation {
private:
    int totalDays;

public:
    Simulation(int days) {
        totalDays = days;
    }

    void run() {
        string cropName;
        cout << "Enter crop name: ";
        cin >> cropName;

        Crop crop(cropName);
        Environment env;

        for (int day = 1; day <= totalDays; day++) {
            cout << "\n--- Day " << day << " ---";

            env.inputConditions();
            float growthRate = env.calculateGrowthRate();

            crop.grow(growthRate);
            crop.displayStatus(day);
        }
    }
};

// -------------------- Main Function --------------------
int main() {
    int days;

    cout << "Enter number of simulation days: ";
    cin >> days;

    // Input validation
    while (days <= 0) {
        cout << "Invalid input! Enter positive number of days: ";
        cin >> days;
    }

    Simulation sim(days);
    sim.run();

    return 0;
}
    

