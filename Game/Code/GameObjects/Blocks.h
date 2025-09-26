#pragma once

#include <GameObjects/Box.h>
#include <memory>
#include <vector>

enum QBlockAnims { DUD, ROTATE };

class QuestionBlock : public Box
{
public:
	explicit QuestionBlock(const Vector2f& initPos);
	~QuestionBlock() override = default;

	void Update(float deltaTime) final;

	void Reset() override;

};

enum RBlockAnims { WAIT, SPIN };
enum ShatterAnims { SCATTER, DROP };

class RotatingBlock : public Box

{
public:
	explicit RotatingBlock(const Vector2f& initPos);
	~RotatingBlock() override = default;
	void Update(float deltaTime) final;

	void Reset() override;

	bool GetJustSmashed() const { return m_justSmashed; }
	void SetJustSmashed(bool justSmashed) { m_justSmashed = justSmashed; }

private:

	void Scatter();

	bool m_justSmashed = false;
};