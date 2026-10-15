#include "download_queue.hpp"
namespace orion {
Task* Queue::find_mutable(const std::string& id) {
    for (auto& task : tasks_) if(task.id == id) return &task;
    return nullptr;
}
const Task* Queue::find(const std::string& id) const {
    for (const auto& task : tasks_) if(task.id == id) return &task;
    return nullptr;
}
bool Queue::add(const std::string& id,const std::string& name,
                const std::string& content_id,std::uint64_t expected_bytes) {
    if (id.empty() || name.empty() || content_id.size()<12 || expected_bytes==0 || find(id)) return false;
    Task t; t.id=id; t.name=name; t.content_id=content_id; t.expected_bytes=expected_bytes;
    tasks_.push_back(t);return true;
}
bool Queue::start(const std::string& id) {
    Task* t=find_mutable(id);if(!t||t->state!=State::Queued)return false;
    t->state=State::Downloading;return true;
}
bool Queue::pause(const std::string& id) {
    Task* t=find_mutable(id);if(!t||t->state!=State::Downloading)return false;
    t->state=State::Paused;return true;
}
bool Queue::resume(const std::string& id) {
    Task* t=find_mutable(id);if(!t||t->state!=State::Paused)return false;
    t->state=State::Downloading;return true;
}
bool Queue::set_progress(const std::string& id,std::uint64_t received) {
    Task* t=find_mutable(id);if(!t||t->state!=State::Downloading||received<t->received_bytes||received>t->expected_bytes)return false;
    t->received_bytes=received;return true;
}
bool Queue::begin_verification(const std::string& id) {
    Task* t=find_mutable(id);if(!t||t->state!=State::Downloading||t->received_bytes!=t->expected_bytes)return false;
    t->state=State::Verifying;return true;
}
bool Queue::finish_verification(const std::string& id,bool hash_matches) {
    Task* t=find_mutable(id);if(!t||t->state!=State::Verifying)return false;
    if(!hash_matches) {t->state=State::Failed;t->error="sha256_mismatch";return false;}
    t->verified=true;t->state=State::Ready;return true;
}
Action Queue::evaluate(const std::string& id, ConsoleState state, bool installer_ready) const {
    const Task* t=find(id);
    if(!t || t->state!=State::Ready || !t->verified) return Action::Reject;
    if(!installer_ready) return Action::WaitForInstaller;
    if(state==ConsoleState::Gaming)return Action::Notify;
    if(state==ConsoleState::Home)return Action::RequestInstall;
    return Action::AskUser;
}
bool Queue::begin_install(const std::string& id, ConsoleState state, bool installer_ready) {
    if(evaluate(id,state,installer_ready)!=Action::RequestInstall)return false;
    // Transition called only AFTER adapter accepts install request.
    Task* t=find_mutable(id);t->state=State::Installing;return true;
}
bool Queue::finish_install(const std::string& id,bool success) {
    Task* t=find_mutable(id);if(!t||t->state!=State::Installing)return false;
    t->state=success?State::Installed:State::Failed;
    if(!success)t->error="installation_failed";
    return success;
}
} // namespace orion
