#include "../src/core/download_queue.hpp"
#include <cassert>
#include <iostream>
using namespace orion;
int main(){
 Queue q;
 assert(q.add("test-app","Homebrew de teste","CONTENTID123456",100));
 assert(!q.add("test-app","Duplicado","CONTENTID123456",100));
 assert(!q.begin_install("test-app",ConsoleState::Home,true));
 assert(q.start("test-app"));
 assert(q.set_progress("test-app",45));
 assert(!q.set_progress("test-app",44));
 assert(!q.begin_verification("test-app"));
 assert(q.pause("test-app") && q.resume("test-app"));
 assert(q.set_progress("test-app",100) && q.begin_verification("test-app"));
 assert(q.finish_verification("test-app",true));
 assert(q.evaluate("test-app",ConsoleState::Gaming,true)==Action::Notify);
 assert(!q.begin_install("test-app",ConsoleState::Gaming,true));
 assert(q.evaluate("test-app",ConsoleState::Unknown,true)==Action::AskUser);
 assert(q.evaluate("test-app",ConsoleState::Home,false)==Action::WaitForInstaller);
 assert(q.evaluate("test-app",ConsoleState::Home,true)==Action::RequestInstall);
 assert(q.begin_install("test-app",ConsoleState::Home,true));
 assert(q.finish_install("test-app",true));
 assert(q.find("test-app")->state==State::Installed);
 Queue corrupted;
 assert(corrupted.add("bad-app","Jogo","CONTENTID123456",10));
 assert(corrupted.start("bad-app")&&corrupted.set_progress("bad-app",10)&&corrupted.begin_verification("bad-app"));
 assert(!corrupted.finish_verification("bad-app",false));
 assert(corrupted.evaluate("bad-app",ConsoleState::Home,true)==Action::Reject);
 std::cout<<"native queue tests: OK\n";
}
