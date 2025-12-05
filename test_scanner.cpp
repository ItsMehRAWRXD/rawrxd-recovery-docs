#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    std::cout << "Simple GGUF Scanner Test\n";
    std::cout << "=========================\n\n";
    
    std::string models_dir = "D:\\OllamaModels";
    if (argc > 1) {
        models_dir = argv[1];
    }
    
    std::cout << "Scanning: " << models_dir << "\n\n";
    
    try {
        std::vector<std::string> models;
        for (const auto& entry : fs::directory_iterator(models_dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".gguf") {
                models.push_back(entry.path().string());
            }
        }
        
        std::sort(models.begin(), models.end(), [](const std::string& a, const std::string& b) {
            return fs::file_size(a) > fs::file_size(b);
        });
        
        std::cout << "Found " << models.size() << " GGUF models:\n\n";
        for (size_t i = 0; i < models.size(); i++) {
            auto path = fs::path(models[i]);
            double size_gb = fs::file_size(models[i]) / (1024.0 * 1024 * 1024);
            std::cout << (i+1) << ". " << path.filename().string() << " (" << size_gb << " GB)\n";
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
