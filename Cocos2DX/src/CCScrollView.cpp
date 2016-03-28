/****************************************************************************
Copyright (c) 2012 cocos2d-x.org
Copyright (c) 2010 Sangwoo Im

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCScrollView.h"
#include "CCActionInterval.h"
#include "CCActionTween.h"
#include "CCActionInstant.h"
#include "CCPointExtension.h"
#include "CCTouch.h"
#include "CCTouchDispatcher.h"
#include "CCGrid.h"
#include "CCDirector.h"

NS_CC_BEGIN

#define SCROLL_DEACCEL_RATE  0.95f
#define SCROLL_DEACCEL_DIST  1.0f
#define BOUNCE_DURATION      0.15f
#define INSET_RATIO          0.2f


CCScrollView::CCScrollView()
	: m_fZoomScale(0.0f)
	, m_fMinZoomScale(0.0f)
	, m_fMaxZoomScale(0.0f)
	, m_pDelegate(NULL)
	, m_bDragging(false)
	, m_bBounceable(false)
	, m_eDirection(kCCScrollViewDirectionBoth)
	, m_bClippingToBounds(false)
	, m_pContainer(NULL)
	, m_bTouchMoved(false)
	, m_fTouchLength(0.0f)
	, m_pTouches(NULL)
	, m_fMinScale(0.0f)
	, m_fMaxScale(0.0f)
{

}

CCScrollView::~CCScrollView()
{
	m_pTouches->release();
}

CCScrollView* CCScrollView::create()
{
	CCScrollView* pScrollView = new CCScrollView();
	if (pScrollView && pScrollView->init())
	{
		pScrollView->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pScrollView);
	}
	return pScrollView;
}

CCScrollView* CCScrollView::create(CCSize size, CCNode* container/* = NULL*/)
{
	CCScrollView *pScrollView = new CCScrollView();
	if (pScrollView && pScrollView->initWithViewSize(size, container))
	{
		pScrollView->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pScrollView);
	}
	return pScrollView;
}

bool CCScrollView::initWithViewSize(CCSize size, CCNode *container/* = NULL*/)
{
	if (CCLayer::init())
	{
		m_pContainer = container;

		if (!m_pContainer)
		{
			m_pContainer = CCLayer::node();
			m_pContainer->setIsRelativeAnchorPoint(true);
			m_pContainer->setAnchorPoint(ccp(0.0f, 0.0f));
		}

		setViewSize(size);
		setTouchEnabled(true);

		m_pTouches = CCArray::array();
		m_pTouches->retain();
		m_pDelegate = NULL;
		m_bBounceable = true;
		m_bClippingToBounds = true;
		m_fMinScale = 1.0f;
		m_fMaxScale = 1.0f;
		m_fTouchLength = 0.0f;
		m_eDirection = kCCScrollViewDirectionBoth;
		m_pContainer->setPosition(ccp(0.0f, 0.0f));

		addChild(m_pContainer);

		return true;
	}
	return false;
}

bool CCScrollView::init()
{
	return initWithViewSize(CCSizeMake(200, 200), NULL);
}

void CCScrollView::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}

bool CCScrollView::isNodeVisible(CCNode* node)
{
	const float scale = getZoomScale();
	const CCSize size = getViewSize();
	const CCPoint offset = getContentOffset();
	CCRect viewRect = CCRectMake(-offset.x/scale, -offset.y/scale, size.width/scale, size.height/scale); 
	return CCRect::CCRectIntersectsRect(viewRect, node->boundingBox());
}

void CCScrollView::pause(CCObject* sender)
{
	m_pContainer->pauseSchedulerAndActions();

	CCObject* pObj = NULL;
	CCArray* pChildren = m_pContainer->getChildren();

	CCARRAY_FOREACH(pChildren, pObj)
	{
		CCNode* pChild = (CCNode*)pObj;
		pChild->pauseSchedulerAndActions();
	}
}

void CCScrollView::resume(CCObject* sender)
{
	CCObject* pObj = NULL;
	CCArray* pChildren = m_pContainer->getChildren();

	CCARRAY_FOREACH(pChildren, pObj)
	{
		CCNode* pChild = (CCNode*)pObj;
		pChild->resumeSchedulerAndActions();
	}

	m_pContainer->resumeSchedulerAndActions();
}

void CCScrollView::setTouchEnabled(bool e)
{
	CCLayer::setIsTouchEnabled(e);
	if (!e)
	{
		m_bDragging = false;
		m_bTouchMoved = false;
		m_pTouches->removeAllObjects();
	}
}

void CCScrollView::setContentOffset(CCPoint offset, bool animated/* = false*/)
{
	if (animated)
	{
		setContentOffsetInDuration(offset, BOUNCE_DURATION);
	} 
	else
	{
		if (!m_bBounceable)
		{
			const CCPoint minOffset = minContainerOffset();
			const CCPoint maxOffset = maxContainerOffset();

			offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
			offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y));
		}

		m_pContainer->setPosition(offset);

		if (m_pDelegate != NULL)
		{
			m_pDelegate->scrollViewDidScroll(this);   
		}
	}
}

void CCScrollView::setContentOffsetInDuration(CCPoint offset, float dt)
{
	CCFiniteTimeAction* scroll = CCMoveTo::actionWithDuration(dt, offset);
	CCFiniteTimeAction* expire = CCCallFuncN::actionWithTarget(this, callfuncN_selector(CCScrollView::stoppedAnimatedScroll));
	m_pContainer->runAction(CCSequence::actionOneTwo(scroll, expire));
	schedule(schedule_selector(CCScrollView::performedAnimatedScroll));
}

CCPoint CCScrollView::getContentOffset()
{
	return m_pContainer->getPosition();
}

void CCScrollView::setZoomScale(float s)
{
	if (m_pContainer->getScale() != s)
	{
		CCPoint offset;
		CCPoint center;
		CCPoint oldCenter, newCenter;

		if (m_fTouchLength == 0.0f) 
		{
			center = ccp(m_tViewSize.width*0.5f, m_tViewSize.height*0.5f);
			center = convertToWorldSpace(center);
		}
		else
		{
			center = m_tTouchPoint;
		}

		oldCenter = m_pContainer->convertToNodeSpace(center);
		m_pContainer->setScale(MAX(m_fMinScale, MIN(m_fMaxScale, s)));
		newCenter = m_pContainer->convertToWorldSpace(oldCenter);
		offset = ccpSub(center, newCenter);

		if (m_pDelegate != NULL)
		{
			m_pDelegate->scrollViewDidZoom(this);
		}

		setContentOffset(ccpAdd(m_pContainer->getPosition(), ccpSub(center, newCenter)));
	}
}

float CCScrollView::getZoomScale()
{
	return m_pContainer->getScale();
}

void CCScrollView::setZoomScale(float s, bool animated)
{
	if (animated)
	{
		setZoomScaleInDuration(s, BOUNCE_DURATION);
	}
	else
	{
		setZoomScale(s);
	}
}

void CCScrollView::setZoomScaleInDuration(float s, float dt)
{
	if (dt > 0)
	{
		if (m_pContainer->getScale() != s)
		{
			CCActionTween *scaleAction = CCActionTween::create(dt, "zoomScale", m_pContainer->getScale(), s);
			runAction(scaleAction);
		}
	}
	else
	{
		setZoomScale(s);
	}
}

CCSize CCScrollView::getViewSize()
{
	return m_tViewSize;
}

void CCScrollView::setViewSize(const CCSize &size)
{
	m_tViewSize = size;
	CCLayer::setContentSize(size);
}

CCNode * CCScrollView::getContainer()
{
	return m_pContainer;
}

void CCScrollView::setContainer(CCNode* pContainer)
{
	removeAllChildrenWithCleanup(true);

	if (!pContainer) return;

	m_pContainer = pContainer;
	m_pContainer->setIsRelativeAnchorPoint(true);
	m_pContainer->setAnchorPoint(ccp(0.0f, 0.0f));

	addChild(m_pContainer);
	setViewSize(m_tViewSize);
}

void CCScrollView::relocateContainer(bool animated)
{
	CCPoint min = minContainerOffset();
	CCPoint max = maxContainerOffset();

	CCPoint oldPoint = m_pContainer->getPosition();

	float newX = oldPoint.x;
	float newY = oldPoint.y;

	if (m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionHorizontal)
	{
		newX = MAX(newX, min.x);
		newX = MIN(newX, max.x);
	}

	if (m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionVertical)
	{
		newY = MIN(newY, max.y);
		newY = MAX(newY, min.y);
	}

	if (newY != oldPoint.y || newX != oldPoint.x)
	{
		setContentOffset(ccp(newX, newY), animated);
	}
}

CCPoint CCScrollView::maxContainerOffset()
{
	return ccp(0.0f, 0.0f);
}

CCPoint CCScrollView::minContainerOffset()
{
	return ccp(
		m_tViewSize.width - m_pContainer->getContentSize().width*m_pContainer->getScaleX(), 
		m_tViewSize.height - m_pContainer->getContentSize().height*m_pContainer->getScaleY());
}

void CCScrollView::deaccelerateScrolling(float dt)
{
	if (m_bDragging)
	{
		unschedule(schedule_selector(CCScrollView::deaccelerateScrolling));
		return;
	}

	float newX, newY;
	CCPoint maxInset, minInset;

	m_pContainer->setPosition(ccpAdd(m_pContainer->getPosition(), m_tScrollDistance));

	if (m_bBounceable)
	{
		maxInset = m_fMaxInset;
		minInset = m_fMinInset;
	}
	else
	{
		maxInset = maxContainerOffset();
		minInset = minContainerOffset();
	}

	newX = MIN(m_pContainer->getPosition().x, maxInset.x);
	newX = MAX(newX, minInset.x);
	newY = MIN(m_pContainer->getPosition().y, maxInset.y);
	newY = MAX(newY, minInset.y);

	newX = m_pContainer->getPosition().x;
	newY = m_pContainer->getPosition().y;

	m_tScrollDistance = ccpSub(m_tScrollDistance, ccp(newX - m_pContainer->getPosition().x, newY - m_pContainer->getPosition().y));
	m_tScrollDistance = ccpMult(m_tScrollDistance, SCROLL_DEACCEL_RATE);
	setContentOffset(ccp(newX,newY));

	if ((fabsf(m_tScrollDistance.x) <= SCROLL_DEACCEL_DIST &&
		fabsf(m_tScrollDistance.y) <= SCROLL_DEACCEL_DIST) ||
		newY > maxInset.y || newY < minInset.y ||
		newX > maxInset.x || newX < minInset.x ||
		newX == maxInset.x || newX == minInset.x ||
		newY == maxInset.y || newY == minInset.y)
	{
		unschedule(schedule_selector(CCScrollView::deaccelerateScrolling));
		relocateContainer(true);
	}
}

void CCScrollView::stoppedAnimatedScroll(CCNode * node)
{
	unschedule(schedule_selector(CCScrollView::performedAnimatedScroll));
}

void CCScrollView::performedAnimatedScroll(float dt)
{
	if (m_bDragging)
	{
		unschedule(schedule_selector(CCScrollView::performedAnimatedScroll));
		return;
	}

	if (m_pDelegate != NULL)
	{
		m_pDelegate->scrollViewDidScroll(this);
	}
}

const CCSize& CCScrollView::getContentSize()
{
	return m_pContainer->getContentSize();
}

void CCScrollView::setContentSize(const CCSize & size)
{
	if (getContainer() != NULL)
	{
		getContainer()->setContentSize(size);
		updateInset();
	}
}

void CCScrollView::updateInset()
{
	if (getContainer() != NULL)
	{
		m_fMaxInset = maxContainerOffset();
		m_fMinInset = minContainerOffset();
		m_fMaxInset = ccp(m_fMaxInset.x + m_tViewSize.width * INSET_RATIO, m_fMaxInset.y + m_tViewSize.height * INSET_RATIO);
		m_fMinInset = ccp(m_fMinInset.x - m_tViewSize.width * INSET_RATIO, m_fMinInset.y - m_tViewSize.height * INSET_RATIO);
	}
}

void CCScrollView::addChild(CCNode * child, int zOrder, int tag)
{
	child->setIsRelativeAnchorPoint(true);
	child->setAnchorPoint(ccp(0.0f, 0.0f));

	if (m_pContainer != child)
	{
		m_pContainer->addChild(child, zOrder, tag);
	}
	else
	{
		CCLayer::addChild(child, zOrder, tag);
	}
}

void CCScrollView::addChild(CCNode * child, int zOrder)
{
	addChild(child, zOrder, child->getTag());
}

void CCScrollView::addChild(CCNode * child)
{
	addChild(child, child->getZOrder(), child->getTag());
}

void CCScrollView::beforeDraw()
{
	if (m_bClippingToBounds)
	{
		float s = getScale();
		CCPoint screenPos = getParent()->convertToWorldSpace(CCPointZero);

		CrossEngine::cglEnable(GL_SCISSOR_TEST);
		CrossEngine::cglScissor((GLint)(screenPos.x*s), (GLint)(screenPos.y*s), (GLsizei)(m_tViewSize.width*s), (GLsizei)(m_tViewSize.height*s));
	}
}

void CCScrollView::afterDraw()
{
	if (m_bClippingToBounds)
	{
		CrossEngine::cglDisable(GL_SCISSOR_TEST);
	}
}

void CCScrollView::visit()
{
	if (!getIsVisible())
	{
		return;
	}

	CrossEngine::cglPushMatrix();

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->beforeDraw();
		transformAncestors();
	}

	transform();
	beforeDraw();

	if(m_pChildren)
	{
		ccArray *arrayData = m_pChildren->data;
		unsigned int i = 0;

		for( ; i < arrayData->num; i++ )
		{
			CCNode *child =  (CCNode*)arrayData->arr[i];
			if ( child->getZOrder() < 0 )
			{
				child->visit();
			}
			else
			{
				break;
			}
		}

		draw();

		for( ; i < arrayData->num; i++ )
		{
			CCNode* child = (CCNode*)arrayData->arr[i];
			child->visit();
		}

	}
	else
	{
		draw();
	}

	afterDraw();
	if ( m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->afterDraw(this);
	}

	CrossEngine::cglPopMatrix();
}

ccTouchClaimed CCScrollView::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if (!getIsVisible())
	{
		return CCTOUCH_CLAIMED_FALSE;
	}

	CCPoint frameOriginal = getParent()->convertToWorldSpace(CCPointZero);
	CCRect frame = CCRectMake(frameOriginal.x, frameOriginal.y, m_tViewSize.width, m_tViewSize.height);

	if (m_pTouches->count() > 2 || m_bTouchMoved || !CCRect::CCRectContainsPoint(frame, m_pContainer->convertToWorldSpace(m_pContainer->convertTouchToNodeSpace(touch))))
	{
		return CCTOUCH_CLAIMED_FALSE;
	}

	if (!m_pTouches->containsObject(touch))
	{
		m_pTouches->addObject(touch);
	}

	if (m_pTouches->count() == 1)
	{
		m_tTouchPoint = convertTouchToNodeSpace(touch);
		m_bTouchMoved = false;
		m_bDragging = true;
		m_tScrollDistance = ccp(0.0f, 0.0f);
		m_tScrollDistanceMax = ccp(0.0f, 0.0f);
		m_tScrollDistanceTotal = ccp(0.0f, 0.0f);
		m_fTouchLength = 0.0f;
	}

	if (m_pTouches->count() == 2)
	{
		m_tTouchPoint  = ccpMidpoint(
			convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
			convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
		m_fTouchLength = ccpDistance(
			m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
			m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
		m_bDragging  = CCTOUCH_CLAIMED_FALSE;
	}

	return CCTOUCH_CLAIMED_TRUE_BREAK;
}

void CCScrollView::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	if (!getIsVisible())
	{
		return;
	}

	if (m_pTouches->containsObject(touch))
	{
		if (m_pTouches->count() == 1 && m_bDragging)
		{
			CCPoint frameOriginal = getParent()->convertToWorldSpace(CCPointZero);
			CCRect frame = CCRectMake(frameOriginal.x, frameOriginal.y, m_tViewSize.width, m_tViewSize.height);

			CCPoint newPoint = convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0));
			CCPoint moveDistance = ccpSub(newPoint, m_tTouchPoint);

			if (CCRect::CCRectContainsPoint(frame, convertToWorldSpace(newPoint)))
			{
				switch (m_eDirection)
				{
				case kCCScrollViewDirectionVertical:
					moveDistance = ccp(0.0f, moveDistance.y);
					break;
				case kCCScrollViewDirectionHorizontal:
					moveDistance = ccp(moveDistance.x, 0.0f);
					break;
				default:
					break;
				}

				CCPoint maxInset = m_fMaxInset;
				CCPoint minInset = m_fMinInset;

				float newX = m_pContainer->getPosition().x + moveDistance.x;
				float newY = m_pContainer->getPosition().y + moveDistance.y;

				m_bTouchMoved  = true;
				m_tTouchPoint = newPoint;
				m_tScrollDistance = moveDistance;

				m_tScrollDistanceTotal.x += moveDistance.x;
				m_tScrollDistanceTotal.y += moveDistance.y;
				m_tScrollDistanceMax.x = max(m_tScrollDistanceMax.x, (float)fabs(m_tScrollDistanceTotal.x));
				m_tScrollDistanceMax.y = max(m_tScrollDistanceMax.y, (float)fabs(m_tScrollDistanceTotal.y));

				setContentOffset(ccp(newX, newY));
			}
		}
		else if (m_pTouches->count() == 2 && !m_bDragging)
		{
			const float len = ccpDistance(
				m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
				m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
			setZoomScale(getZoomScale()*len/m_fTouchLength);
		}
	}
}

void CCScrollView::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	if (!getIsVisible())
	{
		return;
	}

	if (m_pTouches->containsObject(touch))
	{
		if (m_pTouches->count() == 1 && m_bTouchMoved)
		{
			schedule(schedule_selector(CCScrollView::deaccelerateScrolling));
		}
		m_pTouches->removeObject(touch);
	} 

	if (m_pTouches->count() == 0)
	{
		m_bDragging = false;    
		m_bTouchMoved = false;
	}
}

void CCScrollView::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{
	if (!getIsVisible())
	{
		return;
	}

	m_pTouches->removeObject(touch); 

	if (m_pTouches->count() == 0)
	{
		m_bDragging = false;    
		m_bTouchMoved = false;
	}
}

NS_CC_END
