#ifndef SENDER_HH
#define SENDER_HH

#include <string>
#include "AbstractComChannel.hh"
#include <mutex>

class Sender: public AbstractComChannel {
  int _Socket = 0;
  std::mutex _Mutex;

public:
  ~Sender();
  void OpenConnection();
  void Send(const std::string& message) override;
  void CloseConnection();
  bool IsConnected() const { return _Socket != 0; }

  virtual void Init(int Socket) override;
  virtual int GetSocket() const override;
  virtual void LockAccess() override;
  virtual void UnlockAccess() override;   
  virtual std::mutex &UseGuard() override;
};

#endif