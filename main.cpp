#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <queue>

class Activity {
public:
    std::string name;
    std::string category;
    int energyLevel;  // 1-5 scale
    int timeRequired; // minutes
    int priority;     // higher number = higher priority for heap

    Activity(const std::string& n, const std::string& cat, int energy, int time, int prio = 1)
        : name(n), category(cat), energyLevel(energy), timeRequired(time), priority(prio) {
    }

    // For priority queue (max heap)
    bool operator<(const Activity& other) const {
        return priority < other.priority;
    }

    void display() const {
        std::cout << "Activity: " << name << "\n"
            << "Category: " << category << "\n"
            << "Energy Level: " << energyLevel << "/5\n"
            << "Time Required: " << timeRequired << " minutes\n"
            << "Priority: " << priority << "\n" << std::endl;
    }
};

class ActivityGenerator {
private:
    std::priority_queue<Activity> activityHeap;
    std::vector<Activity> allActivities;
    std::mt19937 rng;

    void initializeActivities() {
        // Low energy activities
        allActivities.push_back(Activity("Watch a movie", "Entertainment", 1, 120, 2));
        allActivities.push_back(Activity("Read a book", "Learning", 2, 60, 3));
        allActivities.push_back(Activity("Listen to music", "Relaxation", 1, 30, 1));
        allActivities.push_back(Activity("Meditate", "Wellness", 1, 15, 4));
        allActivities.push_back(Activity("Online shopping", "Entertainment", 1, 45, 1));

        // Medium energy activities
        allActivities.push_back(Activity("Cook a new recipe", "Creative", 3, 90, 3));
        allActivities.push_back(Activity("Organize your room", "Productive", 3, 60, 4));
        allActivities.push_back(Activity("Learn a new skill online", "Learning", 3, 120, 5));
        allActivities.push_back(Activity("Video call a friend", "Social", 2, 45, 3));
        allActivities.push_back(Activity("Write in a journal", "Creative", 2, 30, 4));

        // High energy activities
        allActivities.push_back(Activity("Go for a run", "Exercise", 5, 45, 5));
        allActivities.push_back(Activity("Dance to music", "Exercise", 4, 30, 3));
        allActivities.push_back(Activity("Clean the entire house", "Productive", 4, 180, 4));
        allActivities.push_back(Activity("Start a DIY project", "Creative", 4, 120, 3));
        allActivities.push_back(Activity("Go hiking", "Outdoor", 5, 240, 5));

        // Social activities
        allActivities.push_back(Activity("Plan a hangout with friends", "Social", 3, 30, 4));
        allActivities.push_back(Activity("Join an online community", "Social", 2, 60, 3));
        allActivities.push_back(Activity("Volunteer for a cause", "Social", 3, 120, 5));
    }

public:
    ActivityGenerator() : rng(std::random_device{}()) {
        initializeActivities();
        populateHeap();
    }

    void populateHeap() {
        activityHeap = std::priority_queue<Activity>();
        for (const auto& activity : allActivities) {
            activityHeap.push(activity);
        }
    }

    Activity getRandomActivity() {
        if (allActivities.empty()) return Activity("No activities available", "None", 0, 0, 0);

        std::uniform_int_distribution<int> dist(0, allActivities.size() - 1);
        return allActivities[dist(rng)];
    }

    Activity getTopPriorityActivity() {
        if (activityHeap.empty()) {
            populateHeap();
        }

        Activity top = activityHeap.top();
        activityHeap.pop();
        return top;
    }

    std::vector<Activity> getActivitiesByMood(const std::string& mood) {
        std::vector<Activity> suggestions;

        for (const auto& activity : allActivities) {
            if (mood == "tired" && activity.energyLevel <= 2) {
                suggestions.push_back(activity);
            }
            else if (mood == "energetic" && activity.energyLevel >= 4) {
                suggestions.push_back(activity);
            }
            else if (mood == "productive" && activity.category == "Productive") {
                suggestions.push_back(activity);
            }
            else if (mood == "creative" && activity.category == "Creative") {
                suggestions.push_back(activity);
            }
            else if (mood == "social" && activity.category == "Social") {
                suggestions.push_back(activity);
            }
        }

        // Shuffle the suggestions for variety
        std::shuffle(suggestions.begin(), suggestions.end(), rng);
        return suggestions;
    }

    std::vector<Activity> getActivitiesByTime(int maxTime) {
        std::vector<Activity> suggestions;

        for (const auto& activity : allActivities) {
            if (activity.timeRequired <= maxTime) {
                suggestions.push_back(activity);
            }
        }

        std::shuffle(suggestions.begin(), suggestions.end(), rng);
        return suggestions;
    }

    void addCustomActivity() {
        std::string name, category;
        int energy, time, priority;

        std::cout << "Enter activity name: ";
        std::cin.ignore();
        std::getline(std::cin, name);

        std::cout << "Enter category: ";
        std::getline(std::cin, category);

        std::cout << "Enter energy level (1-5): ";
        std::cin >> energy;

        std::cout << "Enter time required (minutes): ";
        std::cin >> time;

        std::cout << "Enter priority (1-5): ";
        std::cin >> priority;

        allActivities.push_back(Activity(name, category, energy, time, priority));
        populateHeap();

        std::cout << "Activity added successfully!\n" << std::endl;
    }

    void displayAllActivities() const {
        std::cout << "\n=== ALL ACTIVITIES ===\n";
        for (const auto& activity : allActivities) {
            activity.display();
        }
    }
};

void displayMenu() {
    std::cout << "\n=== BOREDOM BUSTER ===\n";
    std::cout << "1. Get random activity\n";
    std::cout << "2. Get highest priority activity\n";
    std::cout << "3. Get activity by mood\n";
    std::cout << "4. Get activity by available time\n";
    std::cout << "5. Add custom activity\n";
    std::cout << "6. View all activities\n";
    std::cout << "7. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    ActivityGenerator generator;
    int choice;

    std::cout << "Welcome to the Boredom Buster!\n";
    std::cout << "Let's find something fun to do!\n";

    while (true) {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
        case 1: {
            Activity activity = generator.getRandomActivity();
            std::cout << "\n=== RANDOM ACTIVITY ===\n";
            activity.display();
            break;
        }

        case 2: {
            Activity activity = generator.getTopPriorityActivity();
            std::cout << "\n=== HIGH PRIORITY ACTIVITY ===\n";
            activity.display();
            break;
        }

        case 3: {
            std::string mood;
            std::cout << "How are you feeling? (tired/energetic/productive/creative/social): ";
            std::cin >> mood;

            auto activities = generator.getActivitiesByMood(mood);
            if (activities.empty()) {
                std::cout << "No activities found for that mood. Try another!\n";
            }
            else {
                std::cout << "\n=== ACTIVITIES FOR YOUR MOOD ===\n";
                for (int i = 0; i < std::min(3, static_cast<int>(activities.size())); ++i) {
                    activities[i].display();
                }
            }
            break;
        }

        case 4: {
            int maxTime;
            std::cout << "How much time do you have? (minutes): ";
            std::cin >> maxTime;

            auto activities = generator.getActivitiesByTime(maxTime);
            if (activities.empty()) {
                std::cout << "No activities found for that time frame.\n";
            }
            else {
                std::cout << "\n=== ACTIVITIES FOR YOUR TIME ===\n";
                for (int i = 0; i < std::min(3, static_cast<int>(activities.size())); ++i) {
                    activities[i].display();
                }
            }
            break;
        }

        case 5: {
            generator.addCustomActivity();
            break;
        }

        case 6: {
            generator.displayAllActivities();
            break;
        }

        case 7: {
            std::cout << "Thanks for using Boredom Buster! Have fun!\n";
            return 0;
        }

        default: {
            std::cout << "Invalid option. Please try again.\n";
            break;
        }
        }
    }

    return 0;
}