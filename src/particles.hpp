#pragma once

class Particle {
 public:
  virtual ~Particle() = 0;
  virtual bool isDirt() const = 0;
  virtual bool isVoid() const = 0;
  virtual void step()
};

class DirtParticle : public Particle {
 public:
  DirtParticle();
  bool isDirt() const override;
  bool isVoid() const override;
  void step() const override;
};

class VoidParticle : public Particle {
 public:
  VoidParticle();
  bool isDirt() const override;
  bool isVoid() const override;
  void step() const override;
};
