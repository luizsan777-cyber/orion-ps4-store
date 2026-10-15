#pragma once
// Portable download/install policy for PS4 homebrew store.
// No access to the Sony/GoldHEN APIs is performed in this module.
#include <cstdint>
#include <string>
#include <vector>

namespace orion {
enum class State { Queued, Downloading, Paused, Verifying, Ready, Installing, Installed, Failed };
enum class ConsoleState { Home, Gaming, Unknown };
enum class Action { None, Notify, RequestInstall, AskUser, WaitForInstaller, Reject };
struct Task {
    std::string id, name, content_id;
    std::uint64_t expected_bytes=0, received_bytes=0;
    State state=State::Queued;
    bool verified=false;
    std::string error;
};
class Queue {
public:
    bool add(const std::string& id, const std::string& name,
             const std::string& content_id, std::uint64_t expected_bytes);
    bool start(const std::string& id);
    bool pause(const std::string& id);
    bool resume(const std::string& id);
    bool set_progress(const std::string& id, std::uint64_t received);
    bool begin_verification(const std::string& id);
    bool finish_verification(const std::string& id, bool hash_matches);
    Action evaluate(const std::string& id, ConsoleState state, bool installer_ready) const;
    bool begin_install(const std::string& id, ConsoleState state, bool installer_ready);
    bool finish_install(const std::string& id, bool success);
    const Task* find(const std::string& id) const;
    const std::vector<Task>& tasks() const { return tasks_; }
private:
    Task* find_mutable(const std::string& id);
    std::vector<Task> tasks_;
};
} // namespace orion
