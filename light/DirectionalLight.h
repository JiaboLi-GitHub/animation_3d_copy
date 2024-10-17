#pragma once

#include <QObject>
#include <QtNodes/NodeDelegateModel>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class Vec3Data;
class DirectionalLight  : public NodeDelegateModel
{
	Q_OBJECT

public:
	DirectionalLight();
	~DirectionalLight() override;

    bool captionVisible() const override;
    QString caption() const override;
    bool portCaptionVisible(PortType portType, PortIndex portIndex) const override;
    QString portCaption(PortType portType, PortIndex portIndex) const override;
    QString name() const override;
    QJsonObject save() const override;
    void load(QJsonObject const& modelJson) override;
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    void setInData(std::shared_ptr<NodeData> nodeData, PortIndex const portIndex) override;
    std::shared_ptr<NodeData> outData(PortIndex const port) override;
    QWidget* embeddedWidget() override;

private:
    std::shared_ptr<Vec3Data> m_direction;
    std::shared_ptr<Vec3Data> m_color;
};
